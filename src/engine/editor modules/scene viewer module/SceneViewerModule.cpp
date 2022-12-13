#include"SceneViewerModule.h"

SceneViewerModule::SceneViewerModule(IViewListener *listener, IRenderTexture* renderTexture, IInputManager* inputManager, Scene* scene, int index)
    :IModule(listener, "Scene viewer", MODULE_TYPE::PERFORMANCE, index),
    renderTexture(renderTexture),
    inputManager(inputManager),
    scene(scene){
        renderTexture->SetCamera(new Camera({0, 2, 5}, {0,0,0}));
    }

void SceneViewerModule::RenderModuleWindow(){
    ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoCollapse;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(800,500));

    if (!ImGui::Begin(GetIdentifier().c_str(), &windowOpened, window_flags))

    
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

    ImGui::PopStyleVar(2);
    const char* renderingModes[] = { "Free", "Aspect Ratio", "Fixed Resolution"};
	const char* combo_preview_value = renderingModes[selectedRenderingMode];

    ImGui::PushItemWidth(150);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));

    if (ImGui::BeginCombo("##renderingMode", combo_preview_value)) {
        //ImDrawList* draw_list = ImGui::GetWindowDrawList();

        // Looping through all the combo entries
        for (int n = 0; n < 3; n++)
        {
            const bool is_selected = (selectedRenderingMode == n);

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
                selectedRenderingMode = n;
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

    if(this->gizmoMode == ImGuizmo::MODE::LOCAL){
        if(ImGui::Button("Local")){
            this->gizmoMode = ImGuizmo::MODE::WORLD;
        }
    }else{
        if(ImGui::Button("World")){
            this->gizmoMode = ImGuizmo::MODE::LOCAL;
        }
    }

    ImGui::SameLine();

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);

    if(ImGui::Button("Translate")){
        this->gizmoType = ImGuizmo::OPERATION::TRANSLATE;
    }

    ImGui::SameLine();

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);

    if(ImGui::Button("Rotate")){
        this->gizmoType = ImGuizmo::OPERATION::ROTATE;
    }

    ImGui::SameLine();

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);

    if(ImGui::Button("Scale")){
        this->gizmoType = ImGuizmo::OPERATION::SCALE;
    }

    availableResolution = glm::ivec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    
    if(selectedRenderingMode == 1){
        ImGui::SameLine(0, 10);
        ImGui::Text("Aspect ratio");
        
        ImGui::SameLine(0, 10);
        ImGui::InputInt("##aspectWidth", &(cAspectRatio[0]));
        ImGui::SameLine(0, 10);
        ImGui::InputInt("##aspectHeight", &(cAspectRatio[1]));

        cAspectRatio[0] = std::max(1, std::min(MAX_RESOLUTION_WIDTH, cAspectRatio[0]));
        cAspectRatio[1] = std::max(1, std::min(MAX_RESOLUTION_HEIGHT, cAspectRatio[1]));
    }else if(selectedRenderingMode == 2){
        ImGui::SameLine(0, 10);
        ImGui::Text("Resolution");
        
        ImGui::SameLine(0, 10);
        ImGui::InputInt2("", cResolution);
        
        cResolution[0] = std::max(MIN_RESOLUTION_WIDTH, std::min(MAX_RESOLUTION_WIDTH, cResolution[0]));
        cResolution[1] = std::max(MIN_RESOLUTION_WIDTH, std::min(MAX_RESOLUTION_WIDTH, cResolution[1]));
    }
    
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();
    ImVec2 minRect = ImGui::GetCursorScreenPos();
    ImVec2 maxRect (ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
                    ImGui::GetCursorScreenPos().y + ImGui::GetContentRegionAvail().y);

    UpdateRenderTexture();
    
    if((RENDER_MODE)selectedRenderingMode != RENDER_MODE::FREE){
        if(((double)availableResolution.x / (double)availableResolution.y) < targetAspectRatio){
            double diff = std::ceil(std::fabs((double)availableResolution.y - (double)availableResolution.x / targetAspectRatio) / 2.0);
            minRect.y +=  diff;
            maxRect.y -= diff;
        }else if(((double)availableResolution.x / (double)availableResolution.y) > targetAspectRatio){
            double diff = std::ceil(std::fabs((double)availableResolution.x - (double)availableResolution.y * targetAspectRatio) / 2.0);
            minRect.x += diff;
            maxRect.x -= diff;
        }
    }
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddImage((ImTextureID)renderTexture->GetTexture(),
        minRect,
        maxRect,
        ImVec2(0, 1),
        ImVec2(1, 0));
    ImVec2 imageSize = ImVec2(renderTexture->GetResolution().x, renderTexture->GetResolution().y);
    drawList->ChannelsMerge();

    ImVec2 hoverRectMin = ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x,
                                    ImGui::GetWindowPos().y + ImGui::GetCursorPos().y);
    
    ImVec2 hoverRectMax = ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x + ImGui::GetContentRegionAvail().x,
                                    ImGui::GetWindowPos().y + ImGui::GetCursorPos().y + ImGui::GetContentRegionAvail().y);
    
    // TODO: Add cursor collision with other windows that may block 
    if(!cameraUpdating && ImGui::IsMouseHoveringRect(hoverRectMin,
                            hoverRectMax)
                        && inputManager->GetMouseDown(MOUSE_BUTTON::RIGHT)){
        ImGui::SetWindowFocus(GetIdentifier().c_str());
        cameraUpdating = true;
        // TODO: Use SDL_WarpMouseInWindow here to lock cursor
    }

    if(cameraUpdating)
        UpdateCamera();

    if(scene->selectedEntities.size() == 1){
        RenderGizmos();
    }
    ImGui::End();

    if(!windowOpened)
        viewListener->RemoveWindow(moduleIndex);
}

ImTextureID SceneViewerModule::GetTextureID(){
    return (ImTextureID)renderTexture->GetTexture();
}

void SceneViewerModule::UpdateCamera(){
	if (inputManager->GetMouse(MOUSE_BUTTON::RIGHT) && ImGui::IsWindowFocused())
	{
		inputManager->SetMouseVisibility(false);
		glm::vec2 mousePos = inputManager->GetMousePos();

		if (lastMousePos != glm::vec2(-1)) {
			deltaMousePos = glm::vec2(mousePos.x - lastMousePos.x, mousePos.y - lastMousePos.y);
		}
		else {
			deltaMousePos = glm::vec2(0);
		}

		lastMousePos = glm::vec2(mousePos.x, mousePos.y);

		if (deltaMousePos.length() > 0) {
			float yAngle = deltaMousePos.x * EditorProperties::cameraRotationSpeed;
			float xAngle = deltaMousePos.y * EditorProperties::cameraRotationSpeed;



            // TODO: Clamp rotation between -90 and 90 degrees
            //renderTexture->GetCamera()->transform.rotation.rotate(renderTexture->GetCamera()->transform.rotation.getQuaternion() * glm::vec3(0,1,0), yAngle);
            //renderTexture->GetCamera()->transform.rotation.rotate(glm::vec3(1,0,0), xAngle);
            TransformComponent& tc = renderTexture->GetCamera()->transform;
            
            //tc.rotation.y += yAngle;
            //tc.rotation.x += xAngle;

            //glm::vec4 vecEuler = glm::vec4(tc.rotation.x, tc.rotation.y, tc.rotation.z, 1.0f);
            //vecEuler = 
            glm::mat4 mat = glm::rotate(glm::mat4(1.0f), xAngle * DEG_2_RAD, glm::vec3(1,0,0));

            glm::vec3 rot;

            rot.x = RAD_2_DEG * atan2f(mat[1][2], mat[2][2]);
            //rot.y = RAD_2_DEG * atan2f(-mat[0][2], sqrtf(mat[1][2] * mat[1][2] + mat[2][2] * mat[2][2]));
            //rot.z = RAD_2_DEG * atan2f(mat[0][1], mat[0][0]);

            tc.rotation.x += rot.x;
            tc.rotation.x = std::min(std::max(tc.rotation.x, -90.0f), 90.0f);

            mat = glm::rotate(glm::mat4(1.0f), yAngle * DEG_2_RAD, glm::vec3(0,1,0));
            //rot.x = RAD_2_DEG * atan2f(mat[1][2], mat[2][2]);
            rot.y = RAD_2_DEG * atan2f(-mat[0][2], sqrtf(mat[1][2] * mat[1][2] + mat[2][2] * mat[2][2]));
            //rot.z = RAD_2_DEG * atan2f(mat[0][1], mat[0][0]);

            tc.rotation.y += rot.y;

            //std::cout << "Camera Euler Angles: x=" << tc.rotation.x << " y=" << tc.rotation.y << " z=" << tc.rotation.z << std::endl;
            /*glm::quat q = renderTexture->GetCamera()->transform.rotation.getQuaternion();

            float yaw = RAD_2_DEG * atan2(2.0f*(q.y*q.z + q.w*q.x), q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z);
            float pitch = RAD_2_DEG * asin(-2.0f*(q.x*q.z - q.w*q.y));
            float roll = RAD_2_DEG * atan2(2.0f*(q.x*q.y + q.w*q.z), q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z);
            
            glm::vec3 clampedRot = renderTexture->GetCamera()->transform.getEulerAngles();*/
            //std::cout << "yaw=" << yaw << " pitch=" << pitch << " roll=" << roll << std::endl;
            //std::cout << "Camera Euler Angles: x=" << clampedRot.x << " y=" << clampedRot.y << " z=" << clampedRot.z << std::endl;
            //clampedRot.x = std::max(std::min(90.0f, clampedRot.x), -90.0f);
            //std::cout << "\tCamera Euler Angles: x=" << clampedRot.x << " y=" << clampedRot.y << " z=" << clampedRot.z << std::endl;

            //renderTexture->GetCamera()->transform.setRotation(renderTexture->GetCamera()->transform.rotation.getEulerAngles());
		}
	}
	else {
		inputManager->SetMouseVisibility(true);
		deltaMousePos = glm::vec2(0);
		lastMousePos = glm::vec2(-1);
        cameraUpdating = false;
	}

	glm::vec3 movementVector = { (inputManager->GetKey(KEYCODE::A) ? -1 : 0) + (inputManager->GetKey(KEYCODE::D) ? 1 : 0),
		(inputManager->GetKey(KEYCODE::Q) ? -1 : 0) + (inputManager->GetKey(KEYCODE::E) ? 1 : 0),
		(inputManager->GetKey(KEYCODE::W) ? -1 : 0) + (inputManager->GetKey(KEYCODE::S) ? 1 : 0) };

	if (glm::length(movementVector) > 0) {
		movementVector = glm::normalize(movementVector);
		movementVector *= viewListener->GetDeltaTime() * EditorProperties::moveSpeed * (inputManager->GetKey(KEYCODE::LEFT_SHIFT) ? 2 : 1);
	}

    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), renderTexture->GetCamera()->transform.rotation.x * DEG_2_RAD, glm::vec3(1.0f, 0.0f, 0.0f))
		    * glm::rotate(glm::mat4(1.0f), renderTexture->GetCamera()->transform.rotation.y * DEG_2_RAD, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec4 deltaVec4 = glm::inverse(rot)
    * glm::vec4(movementVector.x, movementVector.y, movementVector.z, 1.0f);

	renderTexture->GetCamera()->transform.position += glm::vec3(deltaVec4.x, deltaVec4.y, deltaVec4.z);
}

glm::ivec2 SceneViewerModule::CalculateDesiredResolution(glm::ivec2 availableResolution, float targetAspectRatio){
    glm::ivec2 desiredResolution = availableResolution;

    if(((double)availableResolution.x / (double)availableResolution.y) < targetAspectRatio){
        desiredResolution.y = std::ceil((double)availableResolution.x / targetAspectRatio);
    }else if(((double)availableResolution.x / (double)availableResolution.y) > targetAspectRatio){
        desiredResolution.x = std::ceil((double)availableResolution.y * targetAspectRatio);
    }

    return desiredResolution;
}

void SceneViewerModule::StartFrame(){

}

void SceneViewerModule::RenderGizmos(){
    UpdateGizmoType();
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    float windowWidth = (float)ImGui::GetWindowWidth();
    float windowHeight = (float)ImGui::GetWindowHeight();

    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

    TransformComponent& tc = scene->selectedEntities[0].GetComponent<TransformComponent>();
    glm::mat4 transform = tc.getModelMatrix();
    //glm::mat4 translation = glm::mat4(1.0f);

    ImGuizmo::Manipulate(glm::value_ptr(renderTexture->GetCamera()->getViewMatrix()),
        glm::value_ptr(renderTexture->GetCamera()->getProjectionMatrix()),
        this->gizmoType, this->gizmoMode, (float*)glm::value_ptr(transform));

    if(ImGuizmo::IsUsing()){
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
    }
}

void SceneViewerModule::UpdateGizmoType(){
    if(ImGui::IsWindowFocused() && !inputManager->GetMouse(MOUSE_BUTTON::RIGHT)){
        if(inputManager->GetKeyDown(KEYCODE::W)){
            this->gizmoType = ImGuizmo::OPERATION::TRANSLATE;
        }else if(inputManager->GetKeyDown(KEYCODE::E)){
            this->gizmoType = ImGuizmo::OPERATION::ROTATE;
        }else if(inputManager->GetKeyDown(KEYCODE::R)){
            this->gizmoType = ImGuizmo::OPERATION::SCALE;
        }
    }
}

void SceneViewerModule::UpdateRenderTexture(){
    glm::ivec2 desiredResolution;
    
    if(selectedRenderingMode == 0){
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
    }
}