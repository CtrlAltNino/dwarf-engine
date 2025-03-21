#include "Core/Rendering/Material/IMaterial.h"
#include "Core/Scene/Components/SceneComponents.h"
#include "pch.h"
#include <glm/fwd.hpp>
#include <iostream>
#include "ForwardRenderer.h"

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
    : m_RendererApi(rendererApi)
    , m_MaterialFactory(materialFactory)
    , m_ShaderRegistry(shaderRegistry)
    , m_ShaderSourceCollectionFactory(shaderSourceCollectionFactory)
    , m_MeshFactory(meshFactory)
    , m_MeshBufferFactory(meshBufferFactory)
    , m_DrawCallList(drawCallList)
  {
    m_RendererApi->SetClearColor(glm::vec4(0.065f, 0.07f, 0.085, 1.0f));

    m_IdMaterial =
      m_MaterialFactory->CreateMaterial(m_ShaderRegistry->GetOrCreate(
        m_ShaderSourceCollectionFactory->CreateIdShaderSourceCollection()));
    m_IdMaterial->GetShader().Compile();

    m_GridMaterial =
      m_MaterialFactory->CreateMaterial(m_ShaderRegistry->GetOrCreate(
        m_ShaderSourceCollectionFactory->CreateGridShaderSourceCollection()));
    m_GridMaterial->GetShader().Compile();
    m_GridMaterial->GetMaterialProperties().IsDoubleSided = true;
    m_GridMaterial->GetMaterialProperties().IsTransparent = true;

    std::unique_ptr<IMesh> gridMesh = meshFactory->CreateUnitQuad();
    m_GridMeshBuffer = std::move(m_MeshBufferFactory->Create(gridMesh));

    m_GridModelMatrix = glm::mat4(1.0f);
    m_GridModelMatrix = glm::scale(m_GridModelMatrix, glm::vec3(3000.0f));
    m_GridModelMatrix = glm::rotate(
      m_GridModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  }

  ForwardRenderer::~ForwardRenderer()
  {
    m_DrawCallList->Clear();
  }

  void
  ForwardRenderer::Setup(glm::ivec2 viewportSize)
  {
    m_RendererApi->SetViewport(0, 0, viewportSize.x, viewportSize.y);
    // TODO: Create framebuffers etc.
  }

  void
  ForwardRenderer::RenderScene(IScene&    scene,
                               ICamera&   camera,
                               glm::ivec2 viewportSize,
                               bool       renderGrid)
  {
    m_RendererApi->SetClearColor(glm::vec4(0.065f, 0.07f, 0.085, 1.0f));
    m_RendererApi->Clear();
    m_RendererApi->SetViewport(0, 0, viewportSize.x, viewportSize.y);

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
      std::lock_guard<std::mutex> lock(m_DrawCallList->GetMutex());
      for (auto& drawCall : m_DrawCallList->GetDrawCalls())
      {
        if (drawCall->GetMeshBuffer())
        {
          m_RendererApi->RenderIndexed(
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
        m_GridModelMatrix;
      m_RendererApi->RenderIndexed(
        *m_GridMeshBuffer, *m_GridMaterial, camera, translatedGridModelMatrix);
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
    m_RendererApi->Clear(0);

    m_RendererApi->SetViewport(0, 0, viewportSize.x, viewportSize.y);

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
            m_MeshFactory->MergeMeshes(model.Meshes());
          meshRenderer.IdMesh() =
            std::move(m_MeshBufferFactory->Create(mergedMesh));
        }

        glm::mat4    modelMatrix = transform.GetModelMatrix();
        unsigned int id = (unsigned int)entity;
        m_IdMaterial->GetShaderParameters()->SetParameter("objectId", id);
        m_RendererApi->RenderIndexed(
          *meshRenderer.IdMesh(), *m_IdMaterial, camera, modelMatrix);
      }
    }
  }
}