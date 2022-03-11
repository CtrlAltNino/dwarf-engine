#include"SceneViewerModule.h"

SceneViewerModule::SceneViewerModule(IViewListener *listener, IRenderTexture* renderTexture, IInputManager* inputManager, int index)
    :IModule(listener, "Scene viewer", MODULE_TYPE::PERFORMANCE, index),
    camera(new Camera(glm::vec3(0, 2, 5), Quaternion::fromEulerAngles(25, 0, 0))),
    renderTexture(renderTexture),
    inputManager(inputManager){
        renderTexture->SetCamera(camera);
    }

void SceneViewerModule::RenderModuleWindow(){
    ImGuiWindowFlags window_flags = 0;

	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	//window_flags |= ImGuiWindowFlags_NoTitleBar;
	//window_flags |= ImGuiWindowFlags_MenuBar;

    //if (!ImGui::Begin((moduleLabel + "##" + std::to_string(index)).c_str(), NULL, window_flags))
    if (!ImGui::Begin(GetIdentifier().c_str(), &windowOpened, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    //auto app = SSEngine::App::main;
    //uint f_tex = app->getFrameBuffer();
    drawList->AddImage((ImTextureID)renderTexture->GetTexture(),
        pos,
        ImVec2(pos.x + renderTexture->GetResolution().x, pos.y + renderTexture->GetResolution().y),
        ImVec2(0, 1),
        ImVec2(1, 0));
    ImVec2 imageSize = ImVec2(renderTexture->GetResolution().x, renderTexture->GetResolution().y);
    //ImGui::Image((ImTextureID)renderTexture->GetTexture(), imageSize);
    /*if (ImGui::IsItemHovered()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }*/
    /*std::cout << "Identifier: " << GetIdentifier().c_str() << std::endl;
    std::cout << "cameraUpdating: " << cameraUpdating << std::endl;
    std::cout << "Mouse Hovering Rect: " << ImGui::IsMouseHoveringRect(ImGui::GetCursorPos(), imageSize) << std::endl;
    std::cout << "Right mouse down: " << inputManager->GetMouseDown(MOUSE_BUTTON::RIGHT_MOUSE) << std::endl;*/
    //ImGui::IsMouse
    //std::cout << "Cursor pos: " << ImGui::GetCursorPos().x << " | " << ImGui::GetCursorPos().y << std::endl;
    if(!cameraUpdating && ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImVec2(ImGui::GetItemRectMin().x + imageSize.x, ImGui::GetItemRectMin().y + imageSize.y)) && inputManager->GetMouseDown(MOUSE_BUTTON::RIGHT)){
        ImGui::SetWindowFocus(GetIdentifier().c_str());
        cameraUpdating = true;
    }

    //ImGui::
    /*if (ImGui::IsMouseDragging(1)) {
        std::cout << "YEEEEEEEEET" << std::endl;
        //BrowserLinkOpener::OpenLink(GITHUB_LINK);
        viewListener->UpdateEditorCamera(camera);
    }*/

    if(cameraUpdating)
        UpdateCamera();

    glm::vec2 resolution = renderTexture->GetResolution();
    ImVec2 imWSize = ImGui::GetWindowSize();
    glm::vec2 windowSize = glm::vec2(imWSize.x, imWSize.y);
    if(windowSize != resolution){
        renderTexture->SetResolution(windowSize.x, windowSize.y);
        renderTexture->SetAspectRatio(windowSize.x/windowSize.y);
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
			float crsSquared = EditorProperties::cameraRotationSpeed * EditorProperties::cameraRotationSpeed;
			float yAngle = deltaMousePos.x * crsSquared;
			float xAngle = deltaMousePos.y * crsSquared;
			camera->transform.rotate(glm::vec3(0, 1, 0), yAngle);
			camera->transform.rotate(camera->transform.getRotation() * glm::vec3(1, 0, 0), xAngle);
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

	camera->transform.translate(camera->transform.getRotation() * movementVector);
}