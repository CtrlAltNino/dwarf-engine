#include"EditorController.h"

EditorController::EditorController(ProjectData projectData) : editorModel(this), editorView(this){
	std::cout << "1" << std::endl;
	switch(projectData.graphicsApi){
		case GraphicsApi::OpenGL:
			std::cout << "2" << std::endl;
			windowManager = new WindowManagerOpenGL();
			inputManager = new InputManagerOpenGL((WindowManagerOpenGL*)windowManager);
			break;
	}
    windowManager->Init();
	
	// 2. Initialize IMGUI
	//EditorGui::InitGUI();
	//const PerformanceModule *pModule = new PerformanceModule(std::string("Performance statistics"), &(scene.deltaTime));
	//EditorGui::AddWindow(new PerformanceModule(&(scene.deltaTime)));

    // 3. Register assets in file structure

    // 4. Start file watcher

    // 5. Open last scene
	editorModel.InitScene(projectData.path);
    editorView.Init();
    //AddWindow(MODULE_TYPE::PERFORMANCE);

	windowManager->SetWindowName("Dwarf Engine Editor - " + projectData.name + " - " +editorModel.GetScene()->getSceneName() + " (" +(graphicsApiNames[(int)projectData.graphicsApi]) +")");
}

void EditorController::UpdateEditorCamera(Camera* camera){
    Scene* scene = editorModel.GetScene();
	if (inputManager->GetMouseDown(MOUSE_BUTTON::RIGHT_MOUSE))
	{
		//inputManager->SetMouseVisibility(false);
		glm::vec2 mousePos = inputManager->GetMousePos();

		if (scene->lastMousePos != glm::vec2(-1)) {
			scene->deltaMousePos = glm::vec2(mousePos.x - scene->lastMousePos.x, mousePos.y - scene->lastMousePos.y);
		}
		else {
			scene->deltaMousePos = glm::vec2(0);
		}

		scene->lastMousePos = glm::vec2(mousePos.x, mousePos.y);

		if (scene->deltaMousePos.length() > 0) {
			float crsSquared = EditorProperties::cameraRotationSpeed * EditorProperties::cameraRotationSpeed;
			float yAngle = scene->deltaMousePos.x * crsSquared;
			float xAngle = scene->deltaMousePos.y * crsSquared;
			camera->transform.rotate(glm::vec3(0, 1, 0), yAngle);
			camera->transform.rotate(camera->transform.getRotation() * glm::vec3(1, 0, 0), xAngle);
		}
	}
	else {
		inputManager->SetMouseVisibility(true);
		scene->deltaMousePos = glm::vec2(0);
		scene->lastMousePos = glm::vec2(-1);
	}

	glm::vec3 movementVector = { (inputManager->GetKeyDown(KEYCODE::KEYCODE_A) ? -1 : 0) + (inputManager->GetKeyDown(KEYCODE::KEYCODE_D) ? 1 : 0),
		(inputManager->GetKeyDown(KEYCODE::KEYCODE_Q) ? -1 : 0) + (inputManager->GetKeyDown(KEYCODE::KEYCODE_E) ? 1 : 0),
		(inputManager->GetKeyDown(KEYCODE::KEYCODE_W) ? -1 : 0) + (inputManager->GetKeyDown(KEYCODE::KEYCODE_S) ? 1 : 0) };

	if (glm::length(movementVector) > 0) {
		movementVector = glm::normalize(movementVector);
		movementVector *= deltaTime * EditorProperties::moveSpeed * (inputManager->GetKeyDown(KEYCODE::KEYCODE_LEFT_SHIFT) ? 2 : 1);
	}

	camera->transform.translate(camera->transform.getRotation() * movementVector);
}

void EditorController::RunLoop(){
    double lastFrameTime = 0;
	double currentFrameTime = 0;
    Scene* scene = editorModel.GetScene();
	// TODO abstract the close condition
    while (!windowManager->ShouldWindowCloseSignal()) {
		windowManager->StartFrame();
        //editorView.StartFrame();
		//EditorGui::StartFrame();
		
		// ===== Time related stuff
		lastFrameTime = currentFrameTime;
		// TODO abstract the time grabbing
		currentFrameTime = glfwGetTime();
        // Delta time muss woanders vallah
		deltaTime = currentFrameTime - lastFrameTime;

		// Abstract viewport stuff (Context and framebuffer wise)
		glViewport(0, 0, windowManager->GetWidth(), windowManager->GetHeight());
		/*if ((windowManager->GetWidth() != 0) && (windowManager->GetHeight() != 0)) {
			scene->sceneCamera.setAspectRatio((float)windowManager->GetWidth() / windowManager->GetHeight());
		}*/

		// Editor Camera updates
		//UpdateEditorCamera();
		
		// ===== Animation stuff =====
		scene->sceneObjects.at(2).transform.rotate(glm::vec3(0, deltaTime * 88, 0));
		//scene.sceneObjects.at(3).transform.rotate(glm::vec3(0, delta * 180, 0));

		// ===== Drawing Geometry =====
		// TODO: Draw to a framebuffer
		std::vector<IRenderTexture*> *renderTextures = windowManager->GetRenderTextures();
		for(int i = 0; i < renderTextures->size(); i++){
			if ((windowManager->GetWidth() != 0) && (windowManager->GetHeight() != 0)) {
				renderTextures->at(i)->GetCamera()->setAspectRatio((float)renderTextures->at(i)->GetResolution().x / renderTextures->at(i)->GetResolution().y);
			}
			glViewport(0, 0, renderTextures->at(i)->GetResolution().x, renderTextures->at(i)->GetResolution().y);
			//scene->sceneCamera.setAspectRatio(renderTextures->at(i)->GetAspectRatio());
			//scene->sceneCamera.setAspectRatio((float)renderTextures->at(i)->GetResolution().x / (float)renderTextures->at(i)->GetResolution().y);

			renderTextures->at(i)->Bind();
			scene->drawScene(*renderTextures->at(i)->GetCamera());
			renderTextures->at(i)->Unbind();
		}

		// ===== Post processing =====
		// TODO: Implement

		// ===== Gizmo rendering =====
		// TODO: Implement

		// ===== GUI Rendering
        editorView.RenderGui();
		
		// Render modules
		for(int i = 0; i < guiModules.size(); i++){
			guiModules.at(i)->RenderModuleWindow();
		}
        
		//editorView.EndFrame();
		//EditorGui::RenderGUI();
		//EditorGui::EndFrame();

		// ===== Windowing Stuff =====
		windowManager->EndFrame();

		// ===== Framerate managing =====
		while (glfwGetTime() < currentFrameTime + (EditorProperties::FrameLimit != -1 ? 1.0 / EditorProperties::FrameLimit : 0)) {
			// TODO: Update this when implementing multi threading
		}
	}
}

void EditorController::AddWindow(MODULE_TYPE moduleType){
	IModule* guiModule = nullptr;
	switch(moduleType){
		case MODULE_TYPE::PERFORMANCE:
			guiModule = new PerformanceModule((IViewListener*)this, &deltaTime, guiModuleIDCount++);
			break;
		case MODULE_TYPE::SCENE_GRAPH:
			guiModule = new SceneGraphModule((IViewListener*)this, editorModel.GetScene(), guiModuleIDCount++);
			break;
		case MODULE_TYPE::SCENE_VIEWER:
			IRenderTexture* newRT = windowManager->AddRenderTexture();
			guiModule = new SceneViewerModule((IViewListener*)this, newRT, inputManager, guiModuleIDCount++);
			break;
	}

	if(guiModule != nullptr){
		guiModules.push_back(guiModule);
	}
	//editorView.DockWindowToFocused(guiModule->GetModuleName());
}

void EditorController::RemoveWindow(int index){
	for(int i = 0; i < guiModules.size(); i++){
		if(guiModules[i]->GetIndex() == index){
			switch(guiModules[i]->GetModuleType()){
				case MODULE_TYPE::SCENE_VIEWER:
					windowManager->RemoveRenderTexture((int)((SceneViewerModule*)guiModules[i])->GetTextureID());
					break;
			}
			guiModules.erase(guiModules.begin()+i);
		}
	}
}

float EditorController::GetDeltaTime(){
	return deltaTime;
}