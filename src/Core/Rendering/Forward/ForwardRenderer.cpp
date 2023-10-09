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
    ForwardRenderer::~ForwardRenderer() {}

    void ForwardRenderer::RenderScene(Ref<Scene> scene, Ref<Camera> camera, glm::ivec2 viewportSize, boolean renderGrid)
    {
        m_RendererApi->Clear();
        m_RendererApi->SetViewport(0, 0, viewportSize.x, viewportSize.y);

        // Rendering skybox first???

        auto view = scene->GetRegistry()->view<TransformComponent, MeshRendererComponent>();

        for (auto [entity, transform, meshRenderer] : view.each())
        {
            if (meshRenderer.meshAsset != nullptr)
            {
                Ref<ModelAsset> model = AssetDatabase::Retrieve<ModelAsset>(meshRenderer.meshAsset)->GetAsset();
                glm::mat4 modelMatrix = transform.getModelMatrix();

                for (int i = 0; i < model->m_Meshes.size(); i++)
                {
                    if (model->m_Meshes.at(i)->GetMaterialIndex() - 1 < meshRenderer.materialAssets.size())
                    {
                        if (meshRenderer.materialAssets.at(model->m_Meshes.at(i)->GetMaterialIndex() - 1) != nullptr && AssetDatabase::Exists(meshRenderer.materialAssets.at(model->m_Meshes.at(i)->GetMaterialIndex() - 1)))
                        {
                            Ref<MaterialAsset> material = AssetDatabase::Retrieve<MaterialAsset>(meshRenderer.materialAssets.at(model->m_Meshes.at(i)->GetMaterialIndex() - 1))->GetAsset();
                            if (material->m_Material->GetShader() != nullptr && material->m_Material->GetShader()->IsCompiled())
                            {
                                m_RendererApi->RenderIndexed(model->m_Meshes.at(i), material->m_Material, modelMatrix, camera->GetViewMatrix(), camera->GetProjectionMatrix());
                            }
                            else
                            {
                                m_RendererApi->RenderIndexed(model->m_Meshes.at(i), Material::s_ErrorMaterial, modelMatrix, camera->GetViewMatrix(), camera->GetProjectionMatrix());
                            }
                        }
                    }
                }
            }
        }

        // Render grid
        if (renderGrid)
        {
            m_RendererApi->RenderIndexed(Mesh::s_GridMesh, Material::s_GridMaterial, glm::mat4(1.0f), camera->GetViewMatrix(), camera->GetProjectionMatrix());
        }
    }

    Ref<Framebuffer> ForwardRenderer::CreateFramebuffer(glm::ivec2 resolution)
    {
        FramebufferSpecification fbSpec;
        fbSpec.Attachments = {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth};
        fbSpec.Width = resolution.x;
        fbSpec.Height = resolution.y;
        return Framebuffer::Create(fbSpec);
    }
}