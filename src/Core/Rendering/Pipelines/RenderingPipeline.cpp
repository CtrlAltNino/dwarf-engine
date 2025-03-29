#include "pch.h"

#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Core/Rendering/Material/IMaterial.h"
#include "Core/Scene/Components/SceneComponents.h"
#include "RenderingPipeline.h"

namespace Dwarf
{
  RenderingPipeline::RenderingPipeline(
    std::shared_ptr<IRendererApi>     rendererApi,
    std::shared_ptr<IMaterialFactory> materialFactory,
    std::shared_ptr<IShaderRegistry>  shaderRegistry,
    std::shared_ptr<IShaderSourceCollectionFactory>
                                                 shaderSourceCollectionFactory,
    std::shared_ptr<IMeshFactory>                meshFactory,
    std::shared_ptr<IMeshBufferFactory>          meshBufferFactory,
    std::shared_ptr<IFramebufferFactory>         framebufferFactory,
    const std::shared_ptr<IDrawCallListFactory>& drawCallListFactory,
    const std::shared_ptr<IDrawCallWorkerFactory>& drawCallWorkerFactory)
    : mRendererApi(std::move(rendererApi))
    , mMaterialFactory(std::move(materialFactory))
    , mShaderRegistry(std::move(shaderRegistry))
    , mShaderSourceCollectionFactory(std::move(shaderSourceCollectionFactory))
    , mMeshFactory(std::move(meshFactory))
    , mMeshBufferFactory(std::move(meshBufferFactory))
    , mFramebufferFactory(std::move(framebufferFactory))
    , mDrawCallList(drawCallListFactory->Create())
    , mDrawCallWorker(drawCallWorkerFactory->Create(mDrawCallList))
  {
    mRendererApi->SetClearColor(glm::vec4(0.065F, 0.07F, 0.085F, 1.0F));

    mIdMaterial = mMaterialFactory->CreateMaterial(
      mShaderSourceCollectionFactory->CreateIdShaderSourceCollection());
    mIdMaterial->GetShader()->Compile();

    mGridMaterial = mMaterialFactory->CreateMaterial(
      mShaderSourceCollectionFactory->CreateGridShaderSourceCollection());
    mGridMaterial->GetShader()->Compile();
    mGridMaterial->GetMaterialProperties().IsDoubleSided = true;
    mGridMaterial->GetMaterialProperties().IsTransparent = true;

    std::unique_ptr<IMesh> gridMesh = mMeshFactory->CreatePlane();
    mGridMeshBuffer = std::move(mMeshBufferFactory->Create(gridMesh));

    mGridModelMatrix = glm::mat4(1.0F);
    mGridModelMatrix = glm::scale(mGridModelMatrix, glm::vec3(3000.0F));

    mAgxTonemapShader = mShaderRegistry->GetOrCreate(
      mShaderSourceCollectionFactory->CreateAgxTonemapShaderSourceCollection());
    mAgxTonemapShader->Compile();

    // Setup rendering framebuffer according to the pipeline specification
    mFramebuffer = mFramebufferFactory->Create(GetSpecification());

    FramebufferSpecification nonMsaaSpec = GetSpecification();
    nonMsaaSpec.Samples = 1;
    mNonMsaaBuffer = mFramebufferFactory->Create(nonMsaaSpec);

    // Setup framebuffer for presentation
    FramebufferSpecification presentationSpec;
    presentationSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RGBA8 }
    };

    mPresentationBuffer = mFramebufferFactory->Create(presentationSpec);

    // Setup frame buffer for rendering ids
    FramebufferSpecification idSpec;
    idSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RED_INTEGER },
      FramebufferTextureSpecification{ FramebufferTextureFormat::DEPTH }
    };
    idSpec.Width = 512;
    idSpec.Height = 512;
    mIdBuffer = mFramebufferFactory->Create(idSpec);
  }

  RenderingPipeline::~RenderingPipeline()
  {
    mDrawCallList->Clear();
  }

  void
  RenderingPipeline::RenderScene(ICamera& camera, bool renderGrid)
  {
    mFramebuffer->Bind();
    mRendererApi->SetClearColor(glm::vec4(0.065F, 0.07F, 0.085F, 1.0F));
    mRendererApi->Clear();
    mRendererApi->SetViewport(0,
                              0,
                              mFramebuffer->GetSpecification().Width,
                              mFramebuffer->GetSpecification().Height);

    // TODO: Render skybox

    {
      std::lock_guard<std::mutex> lock(mDrawCallList->GetMutex());
      for (auto& drawCall : mDrawCallList->GetDrawCalls())
      {
        if (drawCall->GetMeshBuffer())
        {
          mRendererApi->RenderIndexed(
            *drawCall->GetMeshBuffer(),
            drawCall->GetMaterialAsset().GetMaterial(),
            camera,
            drawCall->GetTransform().GetModelMatrix());
        }
      }
    }

    // Render grid
    if (renderGrid)
    {
      // TODO: Ignore depth testing for grid
      glm::mat4 translatedGridModelMatrix =
        glm::translate(
          glm::mat4(1.0F),
          glm::vec3(camera.GetProperties().Transform.GetPosition().x,
                    0.0F,
                    camera.GetProperties().Transform.GetPosition().z)) *
        mGridModelMatrix;
      mRendererApi->RenderIndexed(
        *mGridMeshBuffer, *mGridMaterial, camera, translatedGridModelMatrix);
    }
    mFramebuffer->Unbind();
  }

  auto
  RenderingPipeline::GetSpecification() const -> FramebufferSpecification
  {
    FramebufferSpecification fbSpec;
    fbSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RGBA16F },
      FramebufferTextureSpecification{ FramebufferTextureFormat::DEPTH }
    };
    fbSpec.Width = 512;
    fbSpec.Height = 512;
    fbSpec.Samples = 1;

    return fbSpec;
  }

  void
  RenderingPipeline::RenderIds(IScene& scene, ICamera& camera)
  {
    mIdBuffer->Bind();
    mIdBuffer->ClearAttachment(0, 0);
    mRendererApi->Clear(0);

    mRendererApi->SetViewport(0,
                              0,
                              mIdBuffer->GetSpecification().Width,
                              mIdBuffer->GetSpecification().Height);

    for (auto view = scene.GetRegistry()
                       .view<TransformComponent, MeshRendererComponent>();
         auto [entity, transform, meshRenderer] : view.each())
    {
      if (meshRenderer.GetModelAsset() && !meshRenderer.IsHidden())
      {
        if (!meshRenderer.IdMesh())
        {
          auto& model =
            dynamic_cast<ModelAsset&>(meshRenderer.GetModelAsset()->GetAsset());
          std::unique_ptr<IMesh> mergedMesh =
            mMeshFactory->MergeMeshes(model.Meshes());
          meshRenderer.IdMesh() =
            std::move(mMeshBufferFactory->Create(mergedMesh));
        }

        glm::mat4 modelMatrix = transform.GetModelMatrix();
        auto      entityId = (uint32_t)entity;
        mIdMaterial->GetShaderParameters()->SetParameter("objectId", entityId);
        mRendererApi->RenderIndexed(
          *meshRenderer.IdMesh(), *mIdMaterial, camera, modelMatrix);
      }
    }
    mIdBuffer->Unbind();

    mRendererApi->Blit(*mFramebuffer,
                       *mNonMsaaBuffer,
                       0,
                       0,
                       mFramebuffer->GetSpecification().Width,
                       mFramebuffer->GetSpecification().Height);

    mRendererApi->CustomBlit(
      *mNonMsaaBuffer, *mPresentationBuffer, 0, 0, mAgxTonemapShader);
  }

  auto
  RenderingPipeline::GetResolution() -> glm::ivec2
  {
    return { mFramebuffer->GetSpecification().Width,
             mFramebuffer->GetSpecification().Height };
  }

  void
  RenderingPipeline::SetResolution(glm::ivec2 resolution)
  {
    mFramebuffer->Resize(resolution.x, resolution.y);
    mNonMsaaBuffer->Resize(resolution.x, resolution.y);
    mIdBuffer->Resize(resolution.x, resolution.y);
    mPresentationBuffer->Resize(resolution.x, resolution.y);
  }

  auto
  RenderingPipeline::GetMsaaSamples() -> int32_t
  {
    return mFramebuffer->GetSpecification().Samples;
  }

  void
  RenderingPipeline::SetMsaaSamples(int32_t samples)
  {
    mFramebuffer->SetSamples(samples);
  }

  auto
  RenderingPipeline::GetMaxMsaaSamples() -> int32_t
  {
    return mRendererApi->GetMaxSamples();
  }

  auto
  RenderingPipeline::ReadPixelId(glm::ivec2 position) -> uint32_t
  {
    return mIdBuffer->ReadPixel(0, position.x, position.y);
  }

  auto
  RenderingPipeline::GetPresentationBufferId() -> uintptr_t
  {
    return mPresentationBuffer->GetColorAttachment()
      .value()
      .get()
      .GetTextureID();
  }
}