#include "dpch.h"

#include "Editor/Modules/Inspector/PreviewRenderer.h"
#include "Input/InputManager.h"

namespace Dwarf
{
    Ref<EditorModel> PreviewRenderer::s_Model = nullptr;
    Ref<Framebuffer> PreviewRenderer::s_Framebuffer = nullptr;
    Ref<Camera> PreviewRenderer::s_Camera = nullptr;
    glm::vec3 PreviewRenderer::s_ModelRotation = {0, 0, 0};
    glm::vec3 PreviewRenderer::s_ModelRotationTarget = {0, 0, 0};
    glm::quat PreviewRenderer::s_ModelRotationQuat = glm::quat({0, 0, 0});
    float PreviewRenderer::s_RotationSpeed = 0.2f;
    float PreviewRenderer::s_ScrollSpeed = 0.3f;
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
            s_Camera->SetAspectRatio((float)size.y / (float)size.x);
            s_Framebuffer->Resize(size.x, size.y);
        }
    }

    void PreviewRenderer::FocusModel(Ref<AssetReference<ModelAsset>> modelAsset)
    {
        std::vector<Dwarf::Ref<Dwarf::Mesh>> vec = modelAsset->GetAsset()->m_Meshes;
        float longestDist = 0;

        for (const auto &subMesh : vec)
        {
            for (Vertex vert : subMesh->GetVertices())
            {
                float dist = glm::length(vert.Position);
                if (dist > longestDist)
                {
                    longestDist = dist;
                }
            }
        }

        s_MaxDistance = longestDist / (tanf(0.5f * s_Camera->GetFov() * DEG_2_RAD));

        s_Camera->GetTransform()->position = {0, 0, 1.3 * s_MaxDistance * s_Distance};
        s_Camera->SetRenderPlaneParameters({0.1f, 1.3 * s_MaxDistance + longestDist});
    }

    void PreviewRenderer::RenderModelPreview(Ref<AssetReference<ModelAsset>> modelAsset)
    {
        if (static entt::entity memory = entt::null; memory != modelAsset->GetHandle())
        {
            FocusModel(modelAsset);
            memory = modelAsset->GetHandle();
            s_ModelRotation = {15, 20, 0};
            s_ModelRotationTarget = {15, 20, 0};
            UpdateRotation({0, 0});
            s_Distance = 1.0f;
        }

        s_Camera->GetTransform()->position = InterpolateVectors(s_Camera->GetTransform()->position, {0, 0, 1.3 * s_MaxDistance * s_Distance}, s_ScrollSpeed);

        s_Framebuffer->Bind();
        Renderer::Get()->RenderModelPreview(modelAsset, s_Camera, {s_Framebuffer->GetSpecification().Width, s_Framebuffer->GetSpecification().Height}, s_ModelRotationQuat);
        s_Framebuffer->Unbind();
    }

    void PreviewRenderer::RenderMaterialPreview(Ref<AssetReference<MaterialAsset>> materialAsset)
    {
        // TODO: Reset sphere rotation when rendering a different material
        s_Camera->GetTransform()->position = {0, 0, 3};
        s_Camera->GetTransform()->rotation = {0, 0, 0};
        s_Camera->SetRenderPlaneParameters({0.1f, 4});

        s_Framebuffer->Bind();
        Renderer::Get()->RenderMaterialPreview(materialAsset, s_Camera, {s_Framebuffer->GetSpecification().Width, s_Framebuffer->GetSpecification().Height}, s_ModelRotationQuat);
        s_Framebuffer->Unbind();
    }

    ImTextureID PreviewRenderer::GetTextureId()
    {
        return (ImTextureID)*s_Framebuffer->GetColorAttachmentRendererID();
    }

    float PreviewRenderer::GetScrollDistance()
    {
        return s_Distance;
    }

    void PreviewRenderer::SetScrollDistance(float distance)
    {
        s_Distance = std::min(std::max(distance, 0.0f), 1.0f);
    }

    float PreviewRenderer::EaseInOutQuad(float t)
    {
        return t * t * (3 - 2 * t);
    }

    glm::vec3 PreviewRenderer::InterpolateVectors(const glm::vec3 &currentVector, const glm::vec3 &targetVector, float speed)
    {
        // Ensure speed is in the range (0, 1] for proper interpolation
        speed = std::max(0.001f, std::min(speed, 1.0f));

        // Calculate the interpolation factor using the easing function
        float t = EaseInOutQuad(speed);

        // Interpolate each component of the vectors
        float x = currentVector.x + t * (targetVector.x - currentVector.x);
        float y = currentVector.y + t * (targetVector.y - currentVector.y);
        float z = currentVector.z + t * (targetVector.z - currentVector.z);

        return {x, y, z};
    }

    void PreviewRenderer::UpdateRotation(glm::vec2 deltaMousePos)
    {
        s_ModelRotationTarget = {std::max(-90.0f, std::min(90.0f, s_ModelRotationTarget.x + deltaMousePos.y)), fmodf(s_ModelRotationTarget.y + deltaMousePos.x, 360.0f), 0};
        s_ModelRotation = InterpolateVectors(s_ModelRotation, s_ModelRotationTarget, s_RotationSpeed);
        s_ModelRotationQuat = glm::rotate(glm::rotate(glm::quat({0, 0, 0}), s_ModelRotation.x * DEG_2_RAD, {1, 0, 0}), s_ModelRotation.y * DEG_2_RAD, {0, 1, 0});
    }
}