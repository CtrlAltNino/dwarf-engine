#include "dpch.h"

#include "Core/Rendering/Forward/ForwardRenderer.h"
#include "Core/Asset/AssetDatabase.h"

namespace Dwarf {
    ForwardRenderer::ForwardRenderer(){
        m_RendererApi = RendererApi::Create();
        m_RendererApi->Init();
        m_RendererApi->SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }
    ForwardRenderer::~ForwardRenderer(){}

    void ForwardRenderer::RenderScene(Ref<Scene> scene, Ref<Camera> camera){
        m_RendererApi->Clear();

        // Rendering skybox first???

        auto view = scene->GetRegistry()->view<TransformComponent, MeshRendererComponent>();

        for(auto [entity, transform, meshRenderer]: view.each())
        {
            if(meshRenderer.meshAsset != nullptr)
            {
                ModelAsset model = AssetDatabase::Retrieve<ModelAsset>(meshRenderer.meshAsset)->GetAsset();
                glm::mat4 modelMatrix = transform.getModelMatrix();

                for(int i = 0; i < model.m_Meshes.size(); i++)
                {
                    std::cout << "Mesh Index: " << i << std::endl;
                    std::cout << "MaterialIndex: " << model.m_Meshes.at(i)->GetMaterialIndex() << std::endl;
                    std::cout << "Material Asset size: " << meshRenderer.materialAssets.size() << std::endl;
                    if(model.m_Meshes.at(i)->GetMaterialIndex()-1 < meshRenderer.materialAssets.size())
                    {
                        std::cout << "Rendering" << std::endl;
                        MaterialAsset material = AssetDatabase::Retrieve<MaterialAsset>(meshRenderer.materialAssets.at(model.m_Meshes.at(i)->GetMaterialIndex()-1))->GetAsset();
                        m_RendererApi->RenderIndexed(model.m_Meshes.at(i), material.m_Material, modelMatrix, camera->GetViewMatrix(), camera->GetProjectionMatrix());
                    }
                }
            }
        }
    }

    Ref<Framebuffer> ForwardRenderer::CreateFramebuffer(glm::ivec2 resolution){
        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8 };
        fbSpec.Width = resolution.x;
        fbSpec.Height = resolution.y;
        return Framebuffer::Create(fbSpec);
    }
}