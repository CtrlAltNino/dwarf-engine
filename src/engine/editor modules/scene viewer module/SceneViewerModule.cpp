#include"SceneViewerModule.h"

SceneViewerModule::SceneViewerModule(IViewListener *listener, IRenderTexture* renderTexture, IInputManager* inputManager, int index)
    :IModule(listener, "Scene viewer", MODULE_TYPE::PERFORMANCE, index),
    renderTexture(renderTexture),
    inputManager(inputManager){
        renderTexture->SetCamera(new Camera(glm::vec3(0, 2, 5), Quaternion::fromEulerAngles(25, 0, 0)));
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
    
    if((RENDER_MODE)selectedRenderingMode != RENDER_MODE::FREE){
        if(((double)availableResolution.x / (double)availableResolution.y) < targetAspectRatio){
            double diff = std::fabs((double)availableResolution.y - (double)availableResolution.x / targetAspectRatio) / 2.0;
            minRect.y += diff;
            maxRect.y -= diff;
        }else if(((double)availableResolution.x / (double)availableResolution.y) > targetAspectRatio){
            double diff = std::fabs((double)availableResolution.x - (double)availableResolution.y * targetAspectRatio) / 2.0;
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
    }

    if(cameraUpdating)
        UpdateCamera();

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
			float crsSquared = EditorProperties::cameraRotationSpeed * EditorProperties::cameraRotationSpeed;
			float yAngle = deltaMousePos.x * crsSquared;
			float xAngle = deltaMousePos.y * crsSquared;
			renderTexture->GetCamera()->transform.rotate(glm::vec3(0, 1, 0), yAngle);
			renderTexture->GetCamera()->transform.rotate(renderTexture->GetCamera()->transform.getRotation() * glm::vec3(1, 0, 0), xAngle);
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

	renderTexture->GetCamera()->transform.translate(renderTexture->GetCamera()->transform.getRotation() * movementVector);
}

glm::ivec2 SceneViewerModule::CalculateDesiredResolution(glm::ivec2 availableResolution, float targetAspectRatio){
    glm::ivec2 desiredResolution = availableResolution;

    if(((double)availableResolution.x / (double)availableResolution.y) < targetAspectRatio){
        desiredResolution.y = (double)availableResolution.x / targetAspectRatio;
    }else if(((double)availableResolution.x / (double)availableResolution.y) > targetAspectRatio){
        desiredResolution.x = (double)availableResolution.y * targetAspectRatio;
    }

    return desiredResolution;
}

void SceneViewerModule::StartFrame(){
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