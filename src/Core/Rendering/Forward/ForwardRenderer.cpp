#include "dpch.h"

#include "Core/Rendering/Forward/ForwardRenderer.h"
#include "Core/Asset/AssetDatabase.h"

namespace Dwarf
{
  ForwardRenderer::ForwardRenderer()
  {
    m_RendererApi = RendererApi::Create();
    m_RendererApi->Init();
    m_RendererApi->SetClearColor(glm::vec4(0.065f, 0.07f, 0.085, 1.0f));
  }
  ForwardRenderer::~ForwardRenderer() = default;

  void
  ForwardRenderer::RenderEntity(Entity&       entity,
                                glm::mat4     viewMatrix,
                                glm::mat4     projectionMatrix,
                                Ref<Material> overrideMaterial = nullptr)
  {
    auto&           transform = entity.GetComponent<TransformComponent>();
    auto&           meshRenderer = entity.GetComponent<MeshRendererComponent>();
    Ref<ModelAsset> model =
      AssetDatabase::Retrieve<ModelAsset>(meshRenderer.meshAsset)->GetAsset();
    glm::mat4 modelMatrix = transform.getModelMatrix();

    for (int i = 0; i < model->m_Meshes.size(); i++)
    {
      if (model->m_Meshes.at(i)->GetMaterialIndex() - 1 <
          meshRenderer.materialAssets.size())
      {
        if (meshRenderer.materialAssets.at(
              model->m_Meshes.at(i)->GetMaterialIndex() - 1) != nullptr &&
            AssetDatabase::Exists(meshRenderer.materialAssets.at(
              model->m_Meshes.at(i)->GetMaterialIndex() - 1)))
        {
          Ref<Material> material =
            overrideMaterial != nullptr
              ? overrideMaterial
              : AssetDatabase::Retrieve<MaterialAsset>(
                  meshRenderer.materialAssets.at(
                    model->m_Meshes.at(i)->GetMaterialIndex() - 1))
                  ->GetAsset()
                  ->m_Material;
          if (material->GetShader() != nullptr &&
              material->GetShader()->IsCompiled())
          {
            m_RendererApi->RenderIndexed(model->m_Meshes.at(i),
                                         material,
                                         modelMatrix,
                                         viewMatrix,
                                         projectionMatrix);
          }
          else
          {
            m_RendererApi->RenderIndexed(model->m_Meshes.at(i),
                                         Material::s_ErrorMaterial,
                                         modelMatrix,
                                         viewMatrix,
                                         projectionMatrix);
          }
        }
      }
    }
  }

  void
  ForwardRenderer::RenderScene(Ref<Scene>  scene,
                               Ref<Camera> camera,
                               glm::ivec2  viewportSize,
                               bool        renderGrid)
  {
    m_RendererApi->SetClearColor(glm::vec4(0.065f, 0.07f, 0.085, 1.0f));
    m_RendererApi->Clear();
    m_RendererApi->SetViewport(0, 0, viewportSize.x, viewportSize.y);
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glm::mat4 projectionMatrix = camera->GetProjectionMatrix();

    // Rendering skybox first???

    for (auto view = scene->GetRegistry()
                       ->view<TransformComponent, MeshRendererComponent>();
         auto [entity, transform, meshRenderer] : view.each())
    {
      if (meshRenderer.meshAsset != nullptr)
      {
        Entity e(entity, scene->GetRegistry());
        RenderEntity(e, viewMatrix, projectionMatrix);
      }
    }

    // Render grid
    if (renderGrid)
    {
      m_RendererApi->RenderIndexed(Mesh::s_GridMesh,
                                   Material::s_GridMaterial,
                                   glm::mat4(1.0f),
                                   camera->GetViewMatrix(),
                                   camera->GetProjectionMatrix());
    }
  }

  void
  ForwardRenderer::RenderIds(Ref<Scene>  scene,
                             Ref<Camera> camera,
                             glm::ivec2  viewportSize)
  {
    m_RendererApi->Clear(0);

    m_RendererApi->SetViewport(0, 0, viewportSize.x, viewportSize.y);

    for (auto view = scene->GetRegistry()
                       ->view<TransformComponent, MeshRendererComponent>();
         auto [entity, transform, meshRenderer] : view.each())
    {
      if (meshRenderer.meshAsset != nullptr)
      {
        Ref<ModelAsset> model =
          AssetDatabase::Retrieve<ModelAsset>(meshRenderer.meshAsset)
            ->GetAsset();
        glm::mat4    modelMatrix = transform.getModelMatrix();
        unsigned int id = (unsigned int)entity;
        Material::s_IdMaterial->SetParameter("objectId", id);

        for (int i = 0; i < model->m_Meshes.size(); i++)
        {
          m_RendererApi->RenderIndexed(model->m_Meshes.at(i),
                                       Material::s_IdMaterial,
                                       modelMatrix,
                                       camera->GetViewMatrix(),
                                       camera->GetProjectionMatrix());
        }
      }
    }
  }

  void
  ForwardRenderer::RenderModelPreview(
    Ref<AssetReference<ModelAsset>> modelAsset,
    Ref<Camera>                     camera,
    glm::ivec2                      viewportSize,
    glm::quat                       rotation)
  {
    m_RendererApi->SetClearColor({ 59 / 255.0f, 66 / 255.0f, 82 / 255.0f, 1 });
    m_RendererApi->Clear();
    m_RendererApi->SetViewport(0, 0, viewportSize.x, viewportSize.y);

    for (int i = 0; i < modelAsset->GetAsset()->m_Meshes.size(); i++)
    {
      m_RendererApi->RenderIndexed(modelAsset->GetAsset()->m_Meshes.at(i),
                                   Material::s_PreviewMaterial,
                                   glm::toMat4(rotation),
                                   camera->GetViewMatrix(),
                                   camera->GetProjectionMatrix());
    }
  }

  void
  ForwardRenderer::RenderMaterialPreview(
    Ref<AssetReference<MaterialAsset>> materialAsset,
    Ref<Camera>                        camera,
    glm::ivec2                         viewportSize,
    glm::quat                          rotation)
  {
    m_RendererApi->SetClearColor({ 59 / 255.0f, 66 / 255.0f, 82 / 255.0f, 1 });
    m_RendererApi->Clear();
    m_RendererApi->SetViewport(0, 0, viewportSize.x, viewportSize.y);

    m_RendererApi->RenderIndexed(Mesh::s_UnitSphere,
                                 materialAsset->GetAsset()->m_Material,
                                 glm::toMat4(rotation),
                                 camera->GetViewMatrix(),
                                 camera->GetProjectionMatrix());
  }

  Ref<Framebuffer>
  ForwardRenderer::CreateFramebuffer(glm::ivec2 resolution)
  {
    FramebufferSpecification fbSpec;
    fbSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RGBA8 },
      FramebufferTextureSpecification{ FramebufferTextureFormat::Depth }
    };
    fbSpec.Width = resolution.x;
    fbSpec.Height = resolution.y;
    return Framebuffer::Create(fbSpec);
  }

  Ref<Framebuffer>
  ForwardRenderer::CreateIDFramebuffer(glm::ivec2 resolution)
  {
    FramebufferSpecification fbSpec;
    fbSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RED_INTEGER },
      FramebufferTextureSpecification{ FramebufferTextureFormat::Depth }
    };
    fbSpec.Width = resolution.x;
    fbSpec.Height = resolution.y;
    return Framebuffer::Create(fbSpec);
  }
}