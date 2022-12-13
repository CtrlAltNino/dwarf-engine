#include"EditorController.h"

EditorController::EditorController(ProjectData projectData) : editorModel(this), editorView(this), assetDatabase(projectData.path) {
	projectPath = projectData.path;
	assetDatabase.Init();
	switch(projectData.graphicsApi){
		case GraphicsApi::OpenGL:
			inputManager = new InputManagerOpenGL();
			windowManager = new WindowManagerOpenGL((InputManagerOpenGL*)inputManager);
			//inputManager = new InputManagerOpenGL((WindowManagerOpenGL*)windowManager);
			break;
	}
    windowManager->Init();
	
	// 2. Initialize IMGUI
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigWindowsMoveFromTitleBarOnly = true;
	//EditorGui::InitGUI();
	//const PerformanceModule *pModule = new PerformanceModule(std::string("Performance statistics"), &(scene.deltaTime));
	//EditorGui::AddWindow(new PerformanceModule(&(scene.deltaTime)));

    // 3. Register assets in file structure

    // 4. Start file watcher

    // 5. Open last scene
	editorModel.InitScene(projectData.path);
    editorView.Init();
    //AddWindow(MODULE_TYPE::PERFORMANCE);

	windowManager->SetWindowTitle("Dwarf Engine Editor - " + projectData.name + " - " +editorModel.GetScene()->getSceneName() + " (" +(graphicsApiNames[(int)projectData.graphicsApi]) +")");
}

void EditorController::RunLoop(){
    double lastFrameTime = 0;
	double currentFrameTime = 0;
    Scene* scene = editorModel.GetScene();
	// TODO abstract the close condition
    while (!windowManager->ShouldWindowCloseSignal()) {
		inputManager->StartFrame();
		windowManager->StartFrame();
        editorView.StartFrame();
		for(int i = 0; i < guiModules.size(); i++){
			guiModules.at(i)->StartFrame();
		}
		
		// ===== Time related stuff
		lastFrameTime = currentFrameTime;
		// TODO abstract the time grabbing
		currentFrameTime = (double)SDL_GetTicks64() / 1000.0f;
        // Delta time muss woanders vallah
		deltaTime = currentFrameTime - lastFrameTime;

		// Abstract viewport stuff (Context and framebuffer wise)
		glViewport(0, 0, windowManager->GetWindowSize().x, windowManager->GetWindowSize().y);
		
		// ===== Animation stuff =====
		//scene->sceneObjects.at(2).transform.rotate(glm::vec3(0, deltaTime * 88, 0));
		//scene.sceneObjects.at(3).transform.rotate(glm::vec3(0, delta * 180, 0));

		// ===== GUI Rendering
        editorView.RenderGui();
		
		// Render modules
		for(int i = 0; i < guiModules.size(); i++){
			guiModules.at(i)->RenderModuleWindow();
		}

		// ===== Drawing Geometry =====
		// TODO: Draw to a framebuffer
		std::vector<IRenderTexture*> *renderTextures = windowManager->GetRenderTextures();
		renderTime = (double)SDL_GetTicks64() / 1000.0f;
		for(int i = 0; i < renderTextures->size(); i++){
			//if ((windowManager->GetWidth() != 0) && (windowManager->GetHeight() != 0)) {
			if (windowManager->GetWindowSize().length() > 0) {
				renderTextures->at(i)->GetCamera()->setAspectRatio((float)renderTextures->at(i)->GetResolution().x / renderTextures->at(i)->GetResolution().y);
			}
			glViewport(0, 0, renderTextures->at(i)->GetResolution().x, renderTextures->at(i)->GetResolution().y);

			renderTextures->at(i)->Bind();
			scene->drawScene(*renderTextures->at(i)->GetCamera());
			renderTextures->at(i)->Unbind();
		}
		renderTime = (double)SDL_GetTicks64() / 1000.0f - renderTime;

		// ===== Post processing =====
		// TODO: Implement

		// ===== Gizmo rendering =====
		// TODO: Implement

		editorView.EndFrame();

		// ===== Windowing Stuff =====
		windowManager->EndFrame();
		//inputManager->UpdatePressStates();

		// ===== Framerate managing =====
		while (((double)SDL_GetTicks64() / 1000.0f) < currentFrameTime + (EditorProperties::FrameLimit != -1 ? 1.0 / EditorProperties::FrameLimit : 0)) {
			// TODO: Update this when implementing multi threading
		}
	}
}

void EditorController::AddWindow(MODULE_TYPE moduleType){
	IModule* guiModule = nullptr;
	switch(moduleType){
		case MODULE_TYPE::PERFORMANCE:
			guiModule = new PerformanceModule((IViewListener*)this, &deltaTime, &renderTime, guiModuleIDCount++);
			break;
		case MODULE_TYPE::SCENE_GRAPH:
			guiModule = new SceneGraphModule((IViewListener*)this, editorModel.GetScene(), inputManager, guiModuleIDCount++);
			break;
		case MODULE_TYPE::SCENE_VIEWER:
			//IRenderTexture* newRT = windowManager->AddRenderTexture();
			guiModule = new SceneViewerModule((IViewListener*)this, windowManager->AddRenderTexture(), inputManager, editorModel.GetScene(), guiModuleIDCount++);
			break;
		case MODULE_TYPE::ASSET_BROWSER:
			guiModule = new AssetBrowserModule((IViewListener*)this, projectPath + "/Assets", guiModuleIDCount++);
			break;
		case MODULE_TYPE::INSPECTOR:
			guiModule = new InspectorModule((IViewListener*)this, editorModel.GetScene(), guiModuleIDCount++);
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
					windowManager->RemoveRenderTexture((intptr_t)((SceneViewerModule*)guiModules[i])->GetTextureID());
					break;
			}
			guiModules.erase(guiModules.begin()+i);
		}
	}
}

float EditorController::GetDeltaTime(){
	return deltaTime;
}