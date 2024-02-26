#include "dpch.h"

#include "Editor/Modules/Inspector/PreviewRenderer.h"
#include "Input/InputManager.h"

namespace Dwarf
{
    Ref<EditorModel> PreviewRenderer::s_Model = nullptr;
    Ref<Framebuffer> PreviewRenderer::s_Framebuffer = nullptr;
    Ref<Camera> PreviewRenderer::s_Camera = nullptr;
    glm::quat PreviewRenderer::s_ModelRotation = glm::quat({45, 45, 0});
    float PreviewRenderer::s_Distance = 1.0f;
    float PreviewRenderer::s_MaxDistance = 0.0f;

    void PreviewRenderer::Init(Ref<EditorModel> model)
    {
        s_Model = model;
        s_Framebuffer = Renderer::Get()->CreateFramebuffer({512, 512});
        s_Camera = CreateRef<Camera>(Camera({0.0f, 0.0f, 0.0f}, {0, 0, 0}, 50.0f, 0.1f, 25000.0f, 1.0f));
    }

    void PreviewRenderer::Resize(glm::ivec2 size)
    {
        if ((s_Framebuffer->GetSpecification().Width != size.x) && (s_Framebuffer->GetSpecification().Height != size.y))
        {
            s_Camera->SetAspectRatio(size.y / size.x);
            s_Framebuffer->Resize(size.x, size.y);
        }
    }

    void PreviewRenderer::FocusModel(Ref<AssetReference<ModelAsset>> modelAsset)
    {
        std::vector<Dwarf::Ref<Dwarf::Mesh>> vec = modelAsset->GetAsset()->m_Meshes;
        float longestDist = 0;

        for (const auto &subMesh : vec)
        {
            for (Vertex vert : *(subMesh->GetVertices()))
            {
                float dist = glm::length(vert.Position);
                if (dist > longestDist)
                {
                    longestDist = dist;
                }
            }
        }

        s_MaxDistance = longestDist / (tanf(0.5f * s_Camera->GetFov() * DEG_2_RAD));

        s_Camera->m_Transform->position = {0, 0, 1.3 * s_MaxDistance * s_Distance};
        s_Camera->SetRenderPlaneParameters({0.1f, 1.3 * s_MaxDistance + longestDist});
    }

    void PreviewRenderer::RenderModelPreview(Ref<AssetReference<ModelAsset>> modelAsset)
    {
        static entt::entity memory = entt::null;

        if (memory != modelAsset->GetHandle())
        {
            FocusModel(modelAsset);
            memory = modelAsset->GetHandle();
            s_ModelRotation = glm::quat({30 * DEG_2_RAD, 0, 0});
            s_Distance = 1.0f;
        }

        s_Distance = std::max(0.0f, std::min(1.0f, s_Distance + InputManager::GetDeltaScroll().y * (float)s_Model->GetDeltaTime()));
        s_Camera->m_Transform->position = {0, 0, 1.3 * s_MaxDistance * s_Distance};

        s_Framebuffer->Bind();
        Renderer::Get()->RenderModelPreview(modelAsset, s_Camera, {s_Framebuffer->GetSpecification().Width, s_Framebuffer->GetSpecification().Height}, s_ModelRotation);
        s_Framebuffer->Unbind();
    }

    void PreviewRenderer::RenderMaterialPreview(Ref<AssetReference<MaterialAsset>> materialAsset)
    {
        s_Camera->m_Transform->position = {0, 0, 3};
        s_Camera->m_Transform->rotation = {0, 0, 0};
        s_Camera->SetRenderPlaneParameters({0.1f, 4});

        s_Framebuffer->Bind();
        Renderer::Get()->RenderMaterialPreview(materialAsset, s_Camera, {s_Framebuffer->GetSpecification().Width, s_Framebuffer->GetSpecification().Height}, s_ModelRotation);
        s_Framebuffer->Unbind();
    }

    ImTextureID PreviewRenderer::GetTextureId()
    {
        return (ImTextureID)*s_Framebuffer->GetColorAttachmentRendererID();
    }

    void PreviewRenderer::UpdateRotation(glm::vec2 deltaMousePos)
    {
        s_ModelRotation = glm::quat({deltaMousePos.y * s_Model->GetDeltaTime(), deltaMousePos.x * s_Model->GetDeltaTime(), 0}) * s_ModelRotation;
    }
}