#include "RenderingPipeline.h"
#include "Core/Rendering/Material/IMaterial.h"
#include "Core/Scene/Components/SceneComponents.h"
#include "pch.h"
#include <glm/fwd.hpp>
#include <utility>

namespace Dwarf
{
  RenderingPipeline::RenderingPipeline(
    std::shared_ptr<IRendererApi>     rendererApi,
    std::shared_ptr<IMaterialFactory> materialFactory,
    std::shared_ptr<IShaderRegistry>  shaderRegistry,
    std::shared_ptr<IShaderSourceCollectionFactory>
                                        shaderSourceCollectionFactory,
    std::shared_ptr<IMeshFactory>       meshFactory,
    std::shared_ptr<IMeshBufferFactory> meshBufferFactory,
    std::shared_ptr<IDrawCallList>      drawCallList)
    : mRendererApi(std::move(rendererApi))
    , mMaterialFactory(std::move(materialFactory))
    , mShaderRegistry(std::move(shaderRegistry))
    , mShaderSourceCollectionFactory(std::move(shaderSourceCollectionFactory))
    , mMeshFactory(std::move(meshFactory))
    , mMeshBufferFactory(std::move(meshBufferFactory))
    , mDrawCallList(std::move(drawCallList))
  {
    mRendererApi->SetClearColor(glm::vec4(0.065F, 0.07F, 0.085F, 1.0F));

    mIdMaterial = mMaterialFactory->CreateMaterial(mShaderRegistry->GetOrCreate(
      mShaderSourceCollectionFactory->CreateIdShaderSourceCollection()));
    mIdMaterial->GetShader().Compile();

    mGridMaterial =
      mMaterialFactory->CreateMaterial(mShaderRegistry->GetOrCreate(
        mShaderSourceCollectionFactory->CreateGridShaderSourceCollection()));
    mGridMaterial->GetShader().Compile();
    mGridMaterial->GetMaterialProperties().IsDoubleSided = true;
    mGridMaterial->GetMaterialProperties().IsTransparent = true;

    std::unique_ptr<IMesh> gridMesh = mMeshFactory->CreateUnitQuad();
    mGridMeshBuffer = std::move(mMeshBufferFactory->Create(gridMesh));

    mGridModelMatrix = glm::mat4(1.0F);
    mGridModelMatrix = glm::scale(mGridModelMatrix, glm::vec3(3000.0F));
    mGridModelMatrix = glm::rotate(
      mGridModelMatrix, glm::radians(-90.0F), glm::vec3(1.0F, 0.0F, 0.0F));
  }

  RenderingPipeline::~RenderingPipeline()
  {
    mDrawCallList->Clear();
  }

  void
  RenderingPipeline::Setup(glm::ivec2 viewportSize)
  {
    mRendererApi->SetViewport(0, 0, viewportSize.x, viewportSize.y);
    // TODO: Create framebuffers etc.
  }

  void
  RenderingPipeline::RenderScene(ICamera&   camera,
                                 glm::ivec2 viewportSize,
                                 bool       renderGrid)
  {
    mRendererApi->SetClearColor(glm::vec4(0.065F, 0.07F, 0.085F, 1.0F));
    mRendererApi->Clear();
    mRendererApi->SetViewport(0, 0, viewportSize.x, viewportSize.y);

    // TODO: Render skybox

    {
      std::lock_guard<std::mutex> lock(mDrawCallList->GetMutex());
      for (auto& drawCall : mDrawCallList->GetDrawCalls())
      {
        if (drawCall->GetMeshBuffer())
        {
          mRendererApi->RenderIndexed(
            *drawCall->GetMeshBuffer(),
            drawCall->GetMaterial(),
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
  RenderingPipeline::RenderIds(IScene&    scene,
                               ICamera&   camera,
                               glm::ivec2 viewportSize)
  {
    mRendererApi->Clear(0);

    mRendererApi->SetViewport(0, 0, viewportSize.x, viewportSize.y);

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
        auto      entityId = (unsigned int)entity;
        mIdMaterial->GetShaderParameters()->SetParameter("objectId", entityId);
        mRendererApi->RenderIndexed(
          *meshRenderer.IdMesh(), *mIdMaterial, camera, modelMatrix);
      }
    }
  }
}