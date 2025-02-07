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
    std::shared_ptr<IShaderFactory>   shaderFactory,
    std::shared_ptr<IShaderSourceCollectionFactory>
                                  shaderSourceCollectionFactory,
    std::shared_ptr<IMeshFactory> meshFactory)
    : m_RendererApi(rendererApi)
    , m_MaterialFactory(materialFactory)
    , m_ShaderFactory(shaderFactory)
    , m_ShaderSourceCollectionFactory(shaderSourceCollectionFactory)
  {
    m_RendererApi->SetClearColor(glm::vec4(0.065f, 0.07f, 0.085, 1.0f));

    m_IdMaterial =
      m_MaterialFactory->CreateMaterial(m_ShaderFactory->CreateShader(
        m_ShaderSourceCollectionFactory->CreateIdShaderSourceCollection()));
    m_IdMaterial->GetShader().Compile();

    m_GridMaterial =
      m_MaterialFactory->CreateMaterial(m_ShaderFactory->CreateShader(
        m_ShaderSourceCollectionFactory->CreateGridShaderSourceCollection()));
    m_GridMaterial->GetShader().Compile();
    m_GridMaterial->GetMaterialProperties().IsDoubleSided = true;
    m_GridMaterial->GetMaterialProperties().IsTransparent = true;

    m_GridMesh = meshFactory->CreateUnitQuad();
    m_GridMesh->SetupMesh();

    m_GridModelMatrix = glm::mat4(1.0f);
    m_GridModelMatrix = glm::scale(m_GridModelMatrix, glm::vec3(3000.0f));
    m_GridModelMatrix = glm::rotate(
      m_GridModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  }

  ForwardRenderer::~ForwardRenderer() = default;

  void
  ForwardRenderer::Setup(glm::ivec2 viewportSize)
  {
    m_RendererApi->SetViewport(0, 0, viewportSize.x, viewportSize.y);
    // TODO: Create framebuffers etc.
  }

  void
  ForwardRenderer::RenderEntity(Entity& entity, ICamera& camera)
  {
    TransformComponent& transform = entity.GetComponent<TransformComponent>();
    MeshRendererComponent& meshRenderer =
      entity.GetComponent<MeshRendererComponent>();
    ModelAsset& model = (ModelAsset&)meshRenderer.GetModelAsset()->GetAsset();
    glm::mat4   modelMatrix = transform.GetModelMatrix();

    for (int i = 0; i < model.Meshes().size(); i++)
    {
      if (model.Meshes().at(i)->GetMaterialIndex() <=
          meshRenderer.MaterialAssets().size())
      {
        if (meshRenderer.MaterialAssets().at(
              model.Meshes().at(i)->GetMaterialIndex()))
        {
          MaterialAsset& materialAsset =
            (MaterialAsset&)meshRenderer.MaterialAssets()
              .at(model.Meshes().at(i)->GetMaterialIndex())
              ->GetAsset();

          m_RendererApi->RenderIndexed(model.Meshes().at(i),
                                       materialAsset.GetMaterial(),
                                       camera,
                                       modelMatrix);
        }
      }
    }
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
    glm::mat4 viewMatrix = camera.GetViewMatrix();
    glm::mat4 projectionMatrix = camera.GetProjectionMatrix();

    // Rendering skybox first???

    for (auto view = scene.GetRegistry()
                       .view<TransformComponent, MeshRendererComponent>();
         auto [entity, transform, meshRenderer] : view.each())
    {
      if (meshRenderer.GetModelAsset() && !meshRenderer.IsHidden())
      {
        Entity e(entity, scene.GetRegistry());
        RenderEntity(e, camera);
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
        m_GridMesh, *m_GridMaterial, camera, translatedGridModelMatrix);
    }
  }

  FramebufferSpecification
  ForwardRenderer::GetSpecification()
  {
    FramebufferSpecification fbSpec;
    fbSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RGBA8 },
      FramebufferTextureSpecification{ FramebufferTextureFormat::DEPTH }
    };
    fbSpec.Width = 512;
    fbSpec.Height = 512;
    fbSpec.Samples = 4;

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
        ModelAsset& model =
          (ModelAsset&)meshRenderer.GetModelAsset()->GetAsset();
        glm::mat4    modelMatrix = transform.GetModelMatrix();
        unsigned int id = (unsigned int)entity;
        m_IdMaterial->GetShaderParameters()->SetParameter("objectId", id);

        for (int i = 0; i < model.Meshes().size(); i++)
        {
          m_RendererApi->RenderIndexed(
            model.Meshes().at(i), *m_IdMaterial, camera, modelMatrix);
        }
      }
    }
  }
}