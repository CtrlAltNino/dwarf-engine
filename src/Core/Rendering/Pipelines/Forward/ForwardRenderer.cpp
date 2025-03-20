#include "ForwardRenderer.h"
#include "Core/Rendering/Material/IMaterial.h"
#include "Core/Scene/Components/SceneComponents.h"
#include "pch.h"
#include <glm/fwd.hpp>
#include <iostream>

namespace Dwarf
{
  ForwardRenderer::ForwardRenderer(
    std::shared_ptr<IRendererApi>     rendererApi,
    std::shared_ptr<IMaterialFactory> materialFactory,
    std::shared_ptr<IShaderRegistry>  shaderRegistry,
    std::shared_ptr<IShaderSourceCollectionFactory>
                                        shaderSourceCollectionFactory,
    std::shared_ptr<IMeshFactory>       meshFactory,
    std::shared_ptr<IMeshBufferFactory> meshBufferFactory,
    std::shared_ptr<IDrawCallList>      drawCallList)
    : mRendererApi(rendererApi)
    , mMaterialFactory(materialFactory)
    , mShaderRegistry(shaderRegistry)
    , mShaderSourceCollectionFactory(shaderSourceCollectionFactory)
    , mMeshFactory(meshFactory)
    , mMeshBufferFactory(meshBufferFactory)
    , mDrawCallList(drawCallList)
  {
    mRendererApi->SetClearColor(glm::vec4(0.065f, 0.07f, 0.085, 1.0f));

    mIdMaterial = mMaterialFactory->CreateMaterial(mShaderRegistry->GetOrCreate(
      mShaderSourceCollectionFactory->CreateIdShaderSourceCollection()));
    mIdMaterial->GetShader().Compile();

    mGridMaterial =
      mMaterialFactory->CreateMaterial(mShaderRegistry->GetOrCreate(
        mShaderSourceCollectionFactory->CreateGridShaderSourceCollection()));
    mGridMaterial->GetShader().Compile();
    mGridMaterial->GetMaterialProperties().IsDoubleSided = true;
    mGridMaterial->GetMaterialProperties().IsTransparent = true;

    std::unique_ptr<IMesh> gridMesh = meshFactory->CreateUnitQuad();
    mGridMeshBuffer = std::move(mMeshBufferFactory->Create(gridMesh));

    mGridModelMatrix = glm::mat4(1.0f);
    mGridModelMatrix = glm::scale(mGridModelMatrix, glm::vec3(3000.0f));
    mGridModelMatrix = glm::rotate(
      mGridModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  }

  ForwardRenderer::~ForwardRenderer()
  {
    mDrawCallList->Clear();
  }

  void
  ForwardRenderer::Setup(glm::ivec2 viewportSize)
  {
    mRendererApi->SetViewport(0, 0, viewportSize.x, viewportSize.y);
    // TODO: Create framebuffers etc.
  }

  void
  ForwardRenderer::RenderScene(IScene&    scene,
                               ICamera&   camera,
                               glm::ivec2 viewportSize,
                               bool       renderGrid)
  {
    mRendererApi->SetClearColor(glm::vec4(0.065f, 0.07f, 0.085, 1.0f));
    mRendererApi->Clear();
    mRendererApi->SetViewport(0, 0, viewportSize.x, viewportSize.y);

    // Rendering skybox first???

    /*for (auto view = scene.GetRegistry()
                       .view<TransformComponent, MeshRendererComponent>();
         auto [entity, transform, meshRenderer] : view.each())
    {
      if (meshRenderer.GetModelAsset() && !meshRenderer.IsHidden())
      {
        Entity e(entity, scene.GetRegistry());
        RenderEntity(e, camera);
      }
    }*/

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
          glm::mat4(1.0f),
          glm::vec3(camera.GetProperties().Transform.GetPosition().x,
                    0.0f,
                    camera.GetProperties().Transform.GetPosition().z)) *
        mGridModelMatrix;
      mRendererApi->RenderIndexed(
        *mGridMeshBuffer, *mGridMaterial, camera, translatedGridModelMatrix);
    }
  }

  FramebufferSpecification
  ForwardRenderer::GetSpecification()
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
  ForwardRenderer::RenderIds(IScene&    scene,
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
          ModelAsset& model =
            (ModelAsset&)meshRenderer.GetModelAsset()->GetAsset();
          std::unique_ptr<IMesh> mergedMesh =
            mMeshFactory->MergeMeshes(model.Meshes());
          meshRenderer.IdMesh() =
            std::move(mMeshBufferFactory->Create(mergedMesh));
        }

        glm::mat4    modelMatrix = transform.GetModelMatrix();
        unsigned int id = (unsigned int)entity;
        mIdMaterial->GetShaderParameters()->SetParameter("objectId", id);
        mRendererApi->RenderIndexed(
          *meshRenderer.IdMesh(), *mIdMaterial, camera, modelMatrix);
      }
    }
  }
}