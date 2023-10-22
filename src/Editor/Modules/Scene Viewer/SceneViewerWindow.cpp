#include "dpch.h"

#include "Editor/Modules/Scene Viewer/SceneViewerWindow.h"
#include "Core/Scene/Scene.h"
#include "Input/InputManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/vec2.hpp>
#include "Editor/Editor.h"

namespace Dwarf
{

    enum class RENDER_MODE
    {
        FREE,
        RESOLUTION,
        ASPECT_RATIO
    };

    SceneViewerWindow::SceneViewerWindow(Ref<EditorModel> model, int index)
        : GuiModule(model, "Scene Viewer", MODULE_TYPE::SCENE_VIEWER, index)
    {
        m_Framebuffer = Renderer::Get()->CreateFramebuffer({512, 512});
        m_Camera = CreateRef<Camera>();
    }

    void SceneViewerWindow::OnUpdate(double deltaTime)
    {
        if (m_Settings.CameraMovement && InputManager::GetMouse(MOUSE_BUTTON::RIGHT))
        {
            m_Camera->OnUpdate(deltaTime);
        }

        // Render scene to the framebuffer with the camera
        m_Framebuffer->Bind();
        Renderer::Get()->RenderScene(m_Model->GetScene(), m_Camera, m_Settings.ViewportSize, m_Settings.RenderGrid);
        m_Framebuffer->Unbind();
    }

    void SceneViewerWindow::OnImGuiRender()
    {
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoCollapse;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(800, 500));

        ImGui::Begin(GetIdentifier().c_str(), &m_WindowOpened, window_flags);

        ImGui::PopStyleVar(2);
        static const char *renderingModes[] = {"Free", "Aspect Ratio", "Fixed Resolution"};

        ImGui::PushItemWidth(150);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        // Rendering mode combo
        if (ImGui::BeginCombo("##renderingMode", renderingModes[m_Settings.RenderingConstraint]))
        {
            for (int n = 0; n < 3; n++)
            {
                const bool is_selected = (m_Settings.RenderingConstraint == n);

                // ==================== Graphics Selectable ====================
                if (ImGui::Selectable(renderingModes[n], is_selected, 0, ImVec2(0, 16 + 10)))
                {
                    m_Settings.RenderingConstraint = (RENDERING_CONSTRAINT)n;
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        ImGui::SameLine(0, 5);

        // Guizmo model toggle
        if (m_Settings.GizmoMode == ImGuizmo::MODE::LOCAL)
        {
            if (ImGui::Button("Local"))
            {
                m_Settings.GizmoMode = ImGuizmo::MODE::WORLD;
            }
        }
        else
        {
            if (ImGui::Button("World"))
            {
                m_Settings.GizmoMode = ImGuizmo::MODE::LOCAL;
            }
        }

        ImGui::SameLine(0, 5);

        // Guizmo type translate button
        if (ImGui::Button("Translate"))
        {
            m_Settings.GizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
        }

        ImGui::SameLine(0, 5);

        // Guizmo type rotate button
        if (ImGui::Button("Rotate"))
        {
            m_Settings.GizmoOperation = ImGuizmo::OPERATION::ROTATE;
        }

        ImGui::SameLine(0, 5);

        // Guizmo type scale button
        if (ImGui::Button("Scale"))
        {
            m_Settings.GizmoOperation = ImGuizmo::OPERATION::SCALE;
        }

        // Rendering mode specific settings input

        if (m_Settings.RenderingConstraint == RENDERING_CONSTRAINT::ASPECT_RATIO)
        {
            ImGui::SameLine(0, 10);
            ImGui::Text("Aspect ratio");

            ImGui::SameLine(0, 10);
            ImGui::InputInt("##aspectWidth", &(m_Settings.AspectRatio[0]));
            ImGui::SameLine(0, 10);
            ImGui::InputInt("##aspectHeight", &(m_Settings.AspectRatio[1]));

            m_Settings.AspectRatio[0] = std::max(1, std::min(MAX_RESOLUTION_WIDTH, m_Settings.AspectRatio[0]));
            m_Settings.AspectRatio[1] = std::max(1, std::min(MAX_RESOLUTION_HEIGHT, m_Settings.AspectRatio[1]));
        }
        else if (m_Settings.RenderingConstraint == RENDERING_CONSTRAINT::FIXED_RESOLUTION)
        {
            ImGui::SameLine(0, 10);
            ImGui::Text("Resolution");
            ImGui::SameLine(0, 10);
            ImGui::InputInt2("", m_Settings.Resolution);
            m_Settings.Resolution[0] = std::max(MIN_RESOLUTION_WIDTH, std::min(MAX_RESOLUTION_WIDTH, m_Settings.Resolution[0]));
            m_Settings.Resolution[1] = std::max(MIN_RESOLUTION_WIDTH, std::min(MAX_RESOLUTION_WIDTH, m_Settings.Resolution[1]));
        }

        ImGui::SameLine(0, 5);

        if (ImGui::Checkbox("Render Grid", &m_Settings.RenderGrid))
            ;

        ImGui::PopStyleVar();
        ImGui::PopItemWidth();

        // Calculating the rect size for the framebuffer
        ImVec2 minRect = ImGui::GetCursorScreenPos();
        ImVec2 maxRect(ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
                       ImGui::GetCursorScreenPos().y + ImGui::GetContentRegionAvail().y);

        // UpdateRenderTexture();
        if ((m_Framebuffer->GetSpecification().Width != ImGui::GetContentRegionAvail().x) || (m_Framebuffer->GetSpecification().Height != ImGui::GetContentRegionAvail().y))
        {
            m_Framebuffer->Resize(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
            m_Settings.ViewportSize = glm::ivec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
            m_Camera->SetAspectRatio((float)ImGui::GetContentRegionAvail().x / (float)ImGui::GetContentRegionAvail().y);
        }

        if ((RENDER_MODE)m_Settings.RenderingConstraint != RENDER_MODE::FREE)
        {
            if (((double)m_Settings.ViewportSize.x / (double)m_Settings.ViewportSize.y) < m_Settings.targetAspectRatio)
            {
                double diff = std::ceil(std::fabs((double)m_Settings.ViewportSize.y - (double)m_Settings.ViewportSize.x / m_Settings.targetAspectRatio) / 2.0);
                minRect.y += diff;
                maxRect.y -= diff;
            }
            else if (((double)m_Settings.ViewportSize.x / (double)m_Settings.ViewportSize.y) > m_Settings.targetAspectRatio)
            {
                double diff = std::ceil(std::fabs((double)m_Settings.ViewportSize.x - (double)m_Settings.ViewportSize.y * m_Settings.targetAspectRatio) / 2.0);
                minRect.x += diff;
                maxRect.x -= diff;
            }
        }

        // Rendering the framebuffer
        ImDrawList *drawList = ImGui::GetWindowDrawList();
        // uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        drawList->AddImage(reinterpret_cast<void *>(GetFrameBufferForImGui()),
                           minRect,
                           maxRect,
                           ImVec2(0, 1),
                           ImVec2(1, 0));
        // ImVec2 imageSize = ImVec2(renderTexture->GetResolution().x, renderTexture->GetResolution().y);
        drawList->ChannelsMerge();

        ImVec2 hoverRectMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x,
                                     ImGui::GetWindowPos().y + ImGui::GetCursorPos().y);

        ImVec2 hoverRectMax = ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x + ImGui::GetContentRegionAvail().x,
                                     ImGui::GetWindowPos().y + ImGui::GetCursorPos().y + ImGui::GetContentRegionAvail().y);

        // TODO: Add cursor collision with other windows that may block

        if (!m_Settings.CameraMovement && ImGui::IsMouseHoveringRect(hoverRectMin, hoverRectMax) && InputManager::GetMouse(MOUSE_BUTTON::RIGHT))
        {
            m_Settings.CameraMovement = true;
            ImGui::FocusWindow(ImGui::GetCurrentWindow());
        }
        else if (m_Settings.CameraMovement && !InputManager::GetMouse(MOUSE_BUTTON::RIGHT))
        {
            m_Settings.CameraMovement = false;
        }

        if (m_Model->m_Selection.selectedEntities.size() == 1)
        {
            RenderGizmos(minRect, maxRect);
        }
        ImGui::End();
    }

    ImTextureID SceneViewerWindow::GetFrameBufferForImGui()
    {
        return (ImTextureID)m_Framebuffer->GetColorAttachmentRendererID();
    }

    glm::ivec2 SceneViewerWindow::CalculateDesiredResolution(glm::ivec2 availableResolution, float targetAspectRatio)
    {
        glm::ivec2 desiredResolution = availableResolution;

        if (((double)availableResolution.x / (double)availableResolution.y) < targetAspectRatio)
        {
            desiredResolution.y = std::ceil((double)availableResolution.x / targetAspectRatio);
        }
        else if (((double)availableResolution.x / (double)availableResolution.y) > targetAspectRatio)
        {
            desiredResolution.x = std::ceil((double)availableResolution.y * targetAspectRatio);
        }

        return desiredResolution;
    }

    void SceneViewerWindow::RenderGizmos(ImVec2 minRect, ImVec2 maxRect)
    {
        UpdateGizmoType();
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        ImGuizmo::SetRect(minRect.x, minRect.y, maxRect.x - minRect.x, maxRect.y - minRect.y);

        TransformComponent &tc = m_Model->m_Selection.selectedEntities.at(0).GetComponent<TransformComponent>();
        glm::mat4 transform = tc.getModelMatrix();

        ImGuizmo::Manipulate(glm::value_ptr(m_Camera->GetViewMatrix()),
                             glm::value_ptr(m_Camera->GetProjectionMatrix()),
                             m_Settings.GizmoOperation, m_Settings.GizmoMode, (float *)glm::value_ptr(transform));

        if (ImGuizmo::IsUsing())
        {
            tc.position = glm::vec3(transform[3]);
            glm::vec3 rotation;
            glm::vec3 originalRotation = tc.rotation;

            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform),
                                                  glm::value_ptr(tc.position),
                                                  glm::value_ptr(rotation),
                                                  glm::value_ptr(tc.scale));

            glm::vec3 deltaRotation = rotation - tc.rotation;

            tc.rotation += deltaRotation;
        }
    }

    void SceneViewerWindow::UpdateGizmoType()
    {
        if (ImGui::IsWindowFocused() && !InputManager::GetMouse(MOUSE_BUTTON::RIGHT))
        {
            if (InputManager::GetKeyDown(KEYCODE::W))
            {
                m_Settings.GizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
            }
            else if (InputManager::GetKeyDown(KEYCODE::E))
            {
                m_Settings.GizmoOperation = ImGuizmo::OPERATION::ROTATE;
            }
            else if (InputManager::GetKeyDown(KEYCODE::R))
            {
                m_Settings.GizmoOperation = ImGuizmo::OPERATION::SCALE;
            }
        }
    }

    glm::vec3 SceneViewerWindow::CalculateSelectionCenter()
    {
        glm::vec3 center = glm::vec3(0);

        for (auto entity : m_Model->m_Selection.selectedEntities)
        {
            center += entity.GetComponent<TransformComponent>().position;
        }

        return center / (float)m_Model->m_Selection.selectedEntities.size();
    }

    std::string SceneViewerWindow::Serialize()
    {
        nlohmann::json state;

        // TODO Serialize scene viewer settings
        // TODO Serialize camera settings
        // state["openedPath"] = m_CurrentDirectory;

        // return state.dump(4);
        return "";
    }
}