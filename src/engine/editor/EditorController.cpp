#include"EditorController.h"

#define EDITOR_INITIAL_WINDOW_WIDTH (1280)
#define EDITOR_INITIAL_WINDOW_HEIGHT (720)

EditorController::EditorController(ProjectData projectData) : editorModel(this), editorView(this), assetDatabase(projectData.path) {
	projectPath = projectData.path;
	assetDatabase.Init();
	switch(projectData.graphicsApi){
		case GraphicsApi::D3D11: break;
		case GraphicsApi::D3D12: break;
		case GraphicsApi::Metal:
				#ifdef __APPLE__
					inputManager = new InputManagerOpenGL();
					windowManager = new WindowManagerMetal((InputManagerOpenGL*)inputManager);
				#endif
			break;
		case GraphicsApi::OpenGL:
			inputManager = new InputManagerOpenGL();
			windowManager = new WindowManagerOpenGL((InputManagerOpenGL*)inputManager);
			//inputManager = new InputManagerOpenGL((WindowManagerOpenGL*)windowManager);
			break;
		case GraphicsApi::Vulkan: break;
	}
    windowManager->Init();

	this->windowTitle = ("Dwarf Engine Editor - " + projectData.name + " - " +editorModel.GetScene()->getSceneName() + " (" +(graphicsApiNames[(int)projectData.graphicsApi]) +")").c_str();
	windowManager->InitWindow({EDITOR_INITIAL_WINDOW_WIDTH, EDITOR_INITIAL_WINDOW_HEIGHT}, {EDITOR_INITIAL_WINDOW_WIDTH, EDITOR_INITIAL_WINDOW_HEIGHT}, this->windowTitle.c_str());
	
    //windowManager->ShowWindow();
    
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

	//windowManager->SetWindowTitle("Dwarf Engine Editor - " + projectData.name + " - " +editorModel.GetScene()->getSceneName() + " (" +(graphicsApiNames[(int)projectData.graphicsApi]) +")");
}

void EditorController::RunLoop(){
	TimeStamp currentFrameStamp = TimeUtilities::GetCurrent();
    TimeStamp lastFrameStamp = TimeUtilities::GetCurrent();
    Scene* scene = editorModel.GetScene();

	windowManager->ShowWindow();
	
	// TODO abstract the close condition
    while (!windowManager->ShouldWindowCloseSignal()) {
		// ===== Time related stuff
		lastFrameStamp = currentFrameStamp;
		// TODO abstract the time grabbing
		currentFrameStamp = TimeUtilities::GetCurrent();
        // Delta time muss woanders vallah
		deltaTime = TimeUtilities::GetDifferenceInSeconds(currentFrameStamp, lastFrameStamp);

		inputManager->StartFrame();
		windowManager->StartFrame();
        editorView.StartFrame();
		for(int i = 0; i < guiModules.size(); i++){
			guiModules.at(i)->StartFrame();
		}

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
		//renderTime = (double)SDL_GetTicks64() / 1000.0f;
		//Uint64 renderStart = SDL_GetTicks64();
		//TimeStamp renderStart = TimeUtilities::GetTimeStamp();
		//Uint64 renderStart = SDL_GetPerformanceCounter();
		TimeStamp renderStart = TimeUtilities::GetCurrent();
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
		
		//renderTime = (SDL_GetTicks64() - renderStart) / 1000;
		//renderTime = TimeUtilities::GetDifferenceInSeconds(TimeUtilities::GetTimeStamp(), renderStart);
		//renderTime = (double)((SDL_GetPerformanceCounter() - renderStart) / (double)(SDL_GetPerformanceFrequency()));
		renderTime = TimeUtilities::GetDifferenceInSeconds(TimeUtilities::GetCurrent(), renderStart);

		// ===== Post processing =====
		// TODO: Implement

		// ===== Gizmo rendering =====
		// TODO: Implement

		editorView.EndFrame();

		// ===== Windowing Stuff =====
		windowManager->EndFrame();
		//inputManager->UpdatePressStates();

		// ===== Framerate managing =====
		//TimeStamp timeout = { currentFrameStamp.milisecondsSinceInit + (1 / EditorProperties::FrameLimit * 1000) };
		//double timeout = (double)currentFrameStamp;
		//timeout += 1000.0 / 144.0;
		//while (((double)SDL_GetTicks64() / 1000.0f) < currentFrameTime + (EditorProperties::FrameLimit != -1 ? 1.0 / EditorProperties::FrameLimit : 0)) {
		//while (EditorProperties::FrameLimit != -1 && (SDL_GetTicks64() < timeout)) {
		while(TimeUtilities::GetDifferenceInSeconds(TimeUtilities::GetCurrent(), currentFrameStamp) < (1.0 / EditorProperties::FrameLimit)){
			// TODO: Update this when implementing multi threading
		}

		//SDL_Delay(1000.0 / (60.0 - (double)((SDL_GetPerformanceCounter() - currentFrameStamp) / (double)(SDL_GetPerformanceFrequency()))));
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
		case MODULE_TYPE::CONSOLE: break;
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
				case MODULE_TYPE::ASSET_BROWSER: break;
				case MODULE_TYPE::CONSOLE: break;
				case MODULE_TYPE::INSPECTOR: break;
				case MODULE_TYPE::PERFORMANCE: break;
				case MODULE_TYPE::SCENE_GRAPH: break;
			}
			guiModules.erase(guiModules.begin()+i);
		}
	}
}

float EditorController::GetDeltaTime(){
	return deltaTime;
}