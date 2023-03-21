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

namespace Dwarf {

    enum class RENDER_MODE {FREE, RESOLUTION, ASPECT_RATIO};
    
    SceneViewerWindow::SceneViewerWindow(Ref<EditorModel> model, int index)
        :GuiModule(model, "Scene Viewer", MODULE_TYPE::PERFORMANCE, index){
            //renderTexture->SetCamera(CreateRef<Camera>(Camera({0, 2, 5}, {0,0,0})));
            FramebufferSpecification fbSpec;
            fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
            fbSpec.Width = 1280;
            fbSpec.Height = 720;
            m_Framebuffer = Framebuffer::Create(fbSpec);
        }

    void SceneViewerWindow::OnUpdate(double deltaTime){
        m_Camera->OnUpdate(deltaTime);
    }

    void SceneViewerWindow::OnImGuiRender(){
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoCollapse;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(800,500));

        if (!ImGui::Begin(GetIdentifier().c_str(), &m_WindowOpened, window_flags))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }

        ImGui::PopStyleVar(2);
        static const char* renderingModes[] = { "Free", "Aspect Ratio", "Fixed Resolution"};
        static const char* combo_preview_value = renderingModes[m_Settings.selectedRenderingMode];

        ImGui::PushItemWidth(150);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));

        if (ImGui::BeginCombo("##renderingMode", combo_preview_value)) {
            //ImDrawList* draw_list = ImGui::GetWindowDrawList();

            // Looping through all the combo entries
            for (int n = 0; n < 3; n++)
            {
                const bool is_selected = (m_Settings.selectedRenderingMode == n);

                // Selectable settings
                /*ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
                ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0));
                ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));*/

                // For drawing a custom Selectable background, we split the channel
                // Now we can draw the text in the foreground, and the colored, rounded rectangle in the background
                //draw_list->ChannelsSplit(2);
                //draw_list->ChannelsSetCurrent(1);

                /*if (n > 0) {
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                }*/

                // ==================== Graphics Selectable ====================
                if (ImGui::Selectable(renderingModes[n], is_selected, 0, ImVec2(0, 16 + 10))) {
                    m_Settings.selectedRenderingMode = n;
                }

                // Reset Style
                //ImGui::PopStyleVar(1);
                //ImGui::PopStyleColor(3);

                // Drawing the background rectangle
                /*if (ImGui::IsItemHovered()) {
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    draw_list->ChannelsSetCurrent(0);
                    ImVec2 p_min = ImGui::GetItemRectMin();
                    ImVec2 p_max = ImGui::GetItemRectMax();
                    ImU32 rectCol = ImGui::IsMouseDown(ImGuiMouseButton_Left) ? IM_COL32(76, 86, 106, 255) : IM_COL32(67, 76, 94, 255);

                    ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, rectCol, 5);
                }
                else if (is_selected) {
                    draw_list->ChannelsSetCurrent(0);
                    ImVec2 p_min = ImGui::GetItemRectMin();
                    ImVec2 p_max = ImGui::GetItemRectMax();
                    ImU32 rectCol = IM_COL32(59, 66, 82, 255);

                    ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, rectCol, 5);
                }*/

                //draw_list->ChannelsMerge();

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        ImGui::SameLine();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);

        if(m_Settings.gizmoMode == ImGuizmo::MODE::LOCAL){
            if(ImGui::Button("Local")){
                m_Settings.gizmoMode = ImGuizmo::MODE::WORLD;
            }
        }else{
            if(ImGui::Button("World")){
                m_Settings.gizmoMode = ImGuizmo::MODE::LOCAL;
            }
        }

        ImGui::SameLine();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);

        if(ImGui::Button("Translate")){
            m_Settings.gizmoType = ImGuizmo::OPERATION::TRANSLATE;
        }

        ImGui::SameLine();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);

        if(ImGui::Button("Rotate")){
            m_Settings.gizmoType = ImGuizmo::OPERATION::ROTATE;
        }

        ImGui::SameLine();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);

        if(ImGui::Button("Scale")){
            m_Settings.gizmoType = ImGuizmo::OPERATION::SCALE;
        }

        m_Settings.availableResolution = glm::ivec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
        
        if(m_Settings.selectedRenderingMode == 1){
            ImGui::SameLine(0, 10);
            ImGui::Text("Aspect ratio");
            
            ImGui::SameLine(0, 10);
            ImGui::InputInt("##aspectWidth", &(m_Settings.cAspectRatio[0]));
            ImGui::SameLine(0, 10);
            ImGui::InputInt("##aspectHeight", &(m_Settings.cAspectRatio[1]));

            m_Settings.cAspectRatio[0] = std::max(1, std::min(MAX_RESOLUTION_WIDTH, m_Settings.cAspectRatio[0]));
            m_Settings.cAspectRatio[1] = std::max(1, std::min(MAX_RESOLUTION_HEIGHT, m_Settings.cAspectRatio[1]));
        }else if(m_Settings.selectedRenderingMode == 2){
            ImGui::SameLine(0, 10);
            ImGui::Text("Resolution");
            
            ImGui::SameLine(0, 10);
            ImGui::InputInt2("", m_Settings.cResolution);
            
            m_Settings.cResolution[0] = std::max(MIN_RESOLUTION_WIDTH, std::min(MAX_RESOLUTION_WIDTH, m_Settings.cResolution[0]));
            m_Settings.cResolution[1] = std::max(MIN_RESOLUTION_WIDTH, std::min(MAX_RESOLUTION_WIDTH, m_Settings.cResolution[1]));
        }
        
        ImGui::PopStyleVar();
        ImGui::PopItemWidth();
        ImVec2 minRect = ImGui::GetCursorScreenPos();
        ImVec2 maxRect (ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
                        ImGui::GetCursorScreenPos().y + ImGui::GetContentRegionAvail().y);

        //UpdateRenderTexture();
        
        if((RENDER_MODE)m_Settings.selectedRenderingMode != RENDER_MODE::FREE){
            if(((double)m_Settings.availableResolution.x / (double)m_Settings.availableResolution.y) < m_Settings.targetAspectRatio){
                double diff = std::ceil(std::fabs((double)m_Settings.availableResolution.y - (double)m_Settings.availableResolution.x / m_Settings.targetAspectRatio) / 2.0);
                minRect.y +=  diff;
                maxRect.y -= diff;
            }else if(((double)m_Settings.availableResolution.x / (double)m_Settings.availableResolution.y) > m_Settings.targetAspectRatio){
                double diff = std::ceil(std::fabs((double)m_Settings.availableResolution.x - (double)m_Settings.availableResolution.y * m_Settings.targetAspectRatio) / 2.0);
                minRect.x += diff;
                maxRect.x -= diff;
            }
        }
        
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        drawList->AddImage(reinterpret_cast<void*>(textureID),
            minRect,
            maxRect,
            ImVec2(0, 1),
            ImVec2(1, 0));
        //ImVec2 imageSize = ImVec2(renderTexture->GetResolution().x, renderTexture->GetResolution().y);
        drawList->ChannelsMerge();

        ImVec2 hoverRectMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x,
                                        ImGui::GetWindowPos().y + ImGui::GetCursorPos().y);
        
        ImVec2 hoverRectMax = ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x + ImGui::GetContentRegionAvail().x,
                                        ImGui::GetWindowPos().y + ImGui::GetCursorPos().y + ImGui::GetContentRegionAvail().y);
        
        // TODO: Add cursor collision with other windows that may block

        static bool cameraMovementStarted = false;

        if(!cameraMovementStarted && ImGui::IsMouseHoveringRect(hoverRectMin, hoverRectMax)
            && InputManager::GetMouse(MOUSE_BUTTON::RIGHT)){
            cameraMovementStarted = true;
            //windowManager->RelativeMouseMode(true);
        }else if(cameraMovementStarted && InputManager::GetMouse(MOUSE_BUTTON::RIGHT)){
            //UpdateCamera();
            //windowManager->LockCursorAtPos(inputManager->GetMousePos());
        }else if(cameraMovementStarted && !InputManager::GetMouse(MOUSE_BUTTON::RIGHT)){
            cameraMovementStarted = false;
            //windowManager->RelativeMouseMode(false);
            //deltaMousePos = glm::vec2(0);
        }

        /*if(scene->selectedEntities.size() == 1){
            RenderGizmos();
        }*/
        ImGui::End();
    }

    ImTextureID SceneViewerWindow::GetTextureID(){
        //return (ImTextureID)renderTexture->GetTexture();
        return nullptr;
    }

    glm::ivec2 SceneViewerWindow::CalculateDesiredResolution(glm::ivec2 availableResolution, float targetAspectRatio){
        glm::ivec2 desiredResolution = availableResolution;

        if(((double)availableResolution.x / (double)availableResolution.y) < targetAspectRatio){
            desiredResolution.y = std::ceil((double)availableResolution.x / targetAspectRatio);
        }else if(((double)availableResolution.x / (double)availableResolution.y) > targetAspectRatio){
            desiredResolution.x = std::ceil((double)availableResolution.y * targetAspectRatio);
        }

        return desiredResolution;
    }

    void SceneViewerWindow::RenderGizmos(){
        /*UpdateGizmoType();
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();

        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

        TransformComponent& tc = m_Model->GetScene()->GetSelectedEntities()->at(0).GetComponent<TransformComponent>();
        glm::mat4 transform = tc.getModelMatrix();*/
        //glm::mat4 translation = glm::mat4(1.0f);

        /*ImGuizmo::Manipulate(glm::value_ptr(renderTexture->GetCamera()->GetViewMatrix()),
            glm::value_ptr(renderTexture->GetCamera()->GetProjectionMatrix()),
            this->gizmoType, this->gizmoMode, (float*)glm::value_ptr(transform));*/

        /*if(ImGuizmo::IsUsing()){
            tc.position = glm::vec3(transform[3]);
            glm::vec3 rotation;
            glm::vec3 originalRotation = tc.rotation;
            
            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform),
                glm::value_ptr(tc.position),
                glm::value_ptr(rotation),
                glm::value_ptr(tc.scale));

            glm::vec3 deltaRotation = rotation - tc.rotation;

            tc.rotation += deltaRotation;
            //tc.rotation = rotation;
        }*/
    }

    void SceneViewerWindow::UpdateGizmoType(){
        if(ImGui::IsWindowFocused() && !InputManager::GetMouse(MOUSE_BUTTON::RIGHT)){
            if(InputManager::GetKeyDown(KEYCODE::W)){
                m_Settings.gizmoType = ImGuizmo::OPERATION::TRANSLATE;
            }else if(InputManager::GetKeyDown(KEYCODE::E)){
                m_Settings.gizmoType = ImGuizmo::OPERATION::ROTATE;
            }else if(InputManager::GetKeyDown(KEYCODE::R)){
                m_Settings.gizmoType = ImGuizmo::OPERATION::SCALE;
            }
        }
    }

    void SceneViewerWindow::UpdateFramebuffer(){
        glm::ivec2 desiredResolution;
        
        /*if(settings.selectedRenderingMode == 0){
            desiredResolution = availableResolution;
            targetAspectRatio = (double)availableResolution.x / (double)availableResolution.y;
        }else if(selectedRenderingMode == 1){
            desiredResolution = CalculateDesiredResolution(availableResolution, (double)cAspectRatio[0] / (double)cAspectRatio[1]);
            targetAspectRatio = (double)desiredResolution.x / (double)desiredResolution.y;
        }else if(selectedRenderingMode == 2){
            desiredResolution = glm::ivec2(cResolution[0], cResolution[1]);
            targetAspectRatio = (double)cResolution[0] / (double)cResolution[1];
        }
        
        if(renderTexture->GetResolution() != desiredResolution){
            renderTexture->SetRenderMode((RENDER_MODE)selectedRenderingMode);
            renderTexture->SetResolution(desiredResolution.x, desiredResolution.y);
            renderTexture->SetAspectRatio(desiredResolution.x/desiredResolution.y);
            renderTexture->UpdateTextureResolution();
        }*/
    }

    glm::vec3 SceneViewerWindow::CalculateSelectionCenter(){
        /*glm::vec3 center = glm::vec3(0);

        for(auto entity : *(m_Model->GetScene()->GetSelectedEntities())){
            center += entity.GetComponent<TransformComponent>().position;
        }

        return center / (float)m_Model->GetScene()->GetSelectedEntities()->size();*/
        return glm::vec3(0);
    }
}