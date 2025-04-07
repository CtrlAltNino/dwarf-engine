#include "pch.h"

#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Core/Rendering/Material/IMaterial.h"
#include "Core/Scene/Components/SceneComponents.h"
#include "IRenderingPipeline.h"
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
    const std::shared_ptr<IDrawCallWorkerFactory>& drawCallWorkerFactory,
    const std::shared_ptr<IPingPongBufferFactory>& pingPongBufferFactory)
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

    SetupRenderFramebuffer();

    SetupPingPongBuffers(pingPongBufferFactory);

    SetupPresentationFramebuffer();

    SetupIdFramebuffer();

    mDrawCallWorker->Invalidate();
  }

  RenderingPipeline::~RenderingPipeline()
  {
    mDrawCallList->Clear();
  }

  void
  RenderingPipeline::SetupRenderFramebuffer()
  {
    FramebufferSpecification spec = GetSpecification();
    mRenderFramebuffer = mFramebufferFactory->Create(spec);
  }

  void
  RenderingPipeline::SetupPingPongBuffers(
    const std::shared_ptr<IPingPongBufferFactory>& pingPongBufferFactory)
  {
    FramebufferSpecification hdrPingPongSpec;
    FramebufferSpecification ldrPingPongSpec;
    hdrPingPongSpec.Attachments.Attachments.emplace_back(
      FramebufferTextureFormat::RGBA16F);
    hdrPingPongSpec.Attachments.Attachments.emplace_back(
      FramebufferTextureFormat::DEPTH);
    ldrPingPongSpec.Attachments.Attachments.emplace_back(
      FramebufferTextureFormat::SRGBA8);

    mHdrPingPong = pingPongBufferFactory->Create(hdrPingPongSpec);
    mLdrPingPong = pingPongBufferFactory->Create(ldrPingPongSpec);
  }

  void
  RenderingPipeline::SetupPresentationFramebuffer()
  {
    FramebufferSpecification presentationSpec;
    presentationSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::SRGBA8 }
    };
    mPresentationBuffer = mFramebufferFactory->Create(presentationSpec);
  }

  void
  RenderingPipeline::SetupIdFramebuffer()
  {
    FramebufferSpecification idSpec;
    idSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RED_INTEGER },
      FramebufferTextureSpecification{ FramebufferTextureFormat::DEPTH }
    };
    mIdBuffer = mFramebufferFactory->Create(idSpec);
  }

  void
  RenderingPipeline::RenderScene(ICamera& camera, GridSettings gridSettings)
  {
    // ==================== Scene Rendering ====================

    mRenderFramebuffer->Bind();
    mRenderFramebuffer->SetDrawBuffer(0);
    mRendererApi->SetViewport(0,
                              0,
                              mRenderFramebuffer->GetSpecification().Width,
                              mRenderFramebuffer->GetSpecification().Height);
    mRendererApi->Clear();

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

    mRenderFramebuffer->Unbind();

    // ==================== Resolve MSAA ====================

    mRendererApi->Blit(*mRenderFramebuffer,
                       mHdrPingPong->GetWriteFramebuffer(),
                       0,
                       0,
                       mRenderFramebuffer->GetSpecification().Width,
                       mRenderFramebuffer->GetSpecification().Height);
    mRendererApi->BlitDepth(*mRenderFramebuffer,
                            mHdrPingPong->GetWriteFramebuffer(),
                            mRenderFramebuffer->GetSpecification().Width,
                            mRenderFramebuffer->GetSpecification().Height);
    mHdrPingPong->Swap();

    // ==================== HDR Post Processing ====================

    // TODO

    // ==================== Tonemapping ====================

    mRendererApi->CustomBlit(mHdrPingPong->GetReadFramebuffer(),
                             mLdrPingPong->GetWriteFramebuffer(),
                             0,
                             0,
                             mTonemapMaterial,
                             false);
    mLdrPingPong->Swap();

    // ==================== LDR Post Processing ====================

    if (gridSettings.RenderGrid)
    {
      mGridMaterial->GetShaderParameters()->SetParameter(
        "uSceneDepth",
        mHdrPingPong->GetReadFramebuffer().GetDepthAttachment().value().get());
      mGridMaterial->GetShaderParameters()->SetParameter(
        "uSceneColor", mLdrPingPong->GetReadTexture());
      mGridMaterial->GetShaderParameters()->SetParameter(
        "uGridHeight", gridSettings.GridYOffset);
      mGridMaterial->GetShaderParameters()->SetParameter(
        "uOpacity", gridSettings.GridOpacity);

      mRendererApi->ApplyPostProcess(
        *mLdrPingPong, camera, *mGridMaterial, true);
      mLdrPingPong->Swap();
    }

    // ==================== Presentation ====================

    mRendererApi->Blit(mLdrPingPong->GetReadFramebuffer(),
                       *mPresentationBuffer,
                       0,
                       0,
                       mRenderFramebuffer->GetSpecification().Width,
                       mRenderFramebuffer->GetSpecification().Height);
  }

  auto
  RenderingPipeline::GetSpecification() const -> FramebufferSpecification
  {
    FramebufferSpecification fbSpec;
    fbSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RGBA16F },
      FramebufferTextureSpecification{ FramebufferTextureFormat::DEPTH }
    };
    fbSpec.Samples = 1;

    return fbSpec;
  }

  void
  RenderingPipeline::RenderIds(IScene& scene, ICamera& camera)
  {
    mIdBuffer->Bind();
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
  }

  auto
  RenderingPipeline::GetResolution() -> glm::ivec2
  {
    return { mRenderFramebuffer->GetSpecification().Width,
             mRenderFramebuffer->GetSpecification().Height };
  }

  void
  RenderingPipeline::SetResolution(glm::ivec2 resolution)
  {
    mRenderFramebuffer->Resize(resolution.x, resolution.y);
    mIdBuffer->Resize(resolution.x, resolution.y);
    mPresentationBuffer->Resize(resolution.x, resolution.y);
    mHdrPingPong->Resize(resolution);
    mLdrPingPong->Resize(resolution);
  }

  auto
  RenderingPipeline::GetMsaaSamples() -> int32_t
  {
    return mRenderFramebuffer->GetSpecification().Samples;
  }

  void
  RenderingPipeline::SetMsaaSamples(int32_t samples)
  {
    mRenderFramebuffer->SetSamples(samples);
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

  void
  RenderingPipeline::SetExposure(float exposure)
  {
    if (mTonemapMaterial)
    {
      mTonemapMaterial->GetShaderParameters()->SetParameter("exposure",
                                                            exposure);
    }
  }

  [[nodiscard]] auto
  RenderingPipeline::GetDrawCallCount() const -> uint32_t
  {
    return mDrawCallList->GetStats().DrawCallCount.load();
  }

  [[nodiscard]] auto
  RenderingPipeline::GetVertexCount() const -> uint32_t
  {
    return mDrawCallList->GetStats().VertexCount.load();
  }

  [[nodiscard]] auto
  RenderingPipeline::GetTriangleCount() const -> uint32_t
  {
    return mDrawCallList->GetStats().TriangleCount.load();
  }

  void
  RenderingPipeline::SetTonemapType(TonemapType type)
  {
    switch (type)
    {
      using enum TonemapType;
      case Reinhard:
        {
          mTonemapMaterial = mMaterialFactory->CreateMaterial(
            mShaderSourceCollectionFactory
              ->CreateReinhardTonemapShaderSourceCollection());
          break;
        }
      case Agx:
        {
          mTonemapMaterial = mMaterialFactory->CreateMaterial(
            mShaderSourceCollectionFactory
              ->CreateAgxTonemapShaderSourceCollection());
          break;
        }
      case Aces:
        {
          mTonemapMaterial = mMaterialFactory->CreateMaterial(
            mShaderSourceCollectionFactory
              ->CreateAcesTonemapShaderSourceCollection());
          break;
        }
    }
  }
}