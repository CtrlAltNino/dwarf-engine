#include"Editor.h"

void Editor::UpdateEditorCamera(){
	if (InputHandler::GetMouseDown(MOUSE_BUTTON::RIGHT_MOUSE))
	{
		InputHandler::SetMouseVisibility(false);
		glm::vec2 mousePos = InputHandler::GetMousePos();

		if (scene.lastMousePos != glm::vec2(-1)) {
			scene.deltaMousePos = glm::vec2(mousePos.x - scene.lastMousePos.x, mousePos.y - scene.lastMousePos.y);
		}
		else {
			scene.deltaMousePos = glm::vec2(0);
		}

		scene.lastMousePos = glm::vec2(mousePos.x, mousePos.y);

		if (scene.deltaMousePos.length() > 0) {
			float crsSquared = EditorProperties::cameraRotationSpeed * EditorProperties::cameraRotationSpeed;
			float yAngle = scene.deltaMousePos.x * crsSquared;
			float xAngle = scene.deltaMousePos.y * crsSquared;
			scene.sceneCamera.transform.rotate(glm::vec3(0, 1, 0), yAngle);
			scene.sceneCamera.transform.rotate(scene.sceneCamera.transform.getRotation() * glm::vec3(1, 0, 0), xAngle);
		}
	}
	else {
		InputHandler::SetMouseVisibility(true);
		scene.deltaMousePos = glm::vec2(0);
		scene.lastMousePos = glm::vec2(-1);
	}

	glm::vec3 movementVector = { (InputHandler::GetKeyDown(KEYCODE::KEYCODE_A) ? -1 : 0) + (InputHandler::GetKeyDown(KEYCODE::KEYCODE_D) ? 1 : 0),
		(InputHandler::GetKeyDown(KEYCODE::KEYCODE_Q) ? -1 : 0) + (InputHandler::GetKeyDown(KEYCODE::KEYCODE_E) ? 1 : 0),
		(InputHandler::GetKeyDown(KEYCODE::KEYCODE_W) ? -1 : 0) + (InputHandler::GetKeyDown(KEYCODE::KEYCODE_S) ? 1 : 0) };

	if (glm::length(movementVector) > 0) {
		movementVector = glm::normalize(movementVector);
		movementVector *= scene.deltaTime * EditorProperties::moveSpeed * (InputHandler::GetKeyDown(KEYCODE::KEYCODE_LEFT_SHIFT) ? 2 : 1);
	}

	scene.sceneCamera.transform.translate(scene.sceneCamera.transform.getRotation() * movementVector);
}

void Editor::InitScene(std::string projectPath) {
	// Creating Objects
	BasicMaterial cubeMat = BasicMaterial("data/engine/shaders/default/opengl/default");
	cubeMat.setAlbedoMap((projectPath + "/Assets/textures/popcat.jpg").c_str());
	cubeMat.settings.cullMode = GL_BACK;
	cubeMat.settings.cullFaces = true;
	
	Transform cubeTransform = Transform(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0));
	Renderer cubeRenderer = Renderer(&cubeMat, (projectPath + "/Assets/models/cube.obj").c_str());
	Object cube = Object("Popcat cube", &cubeTransform, &cubeRenderer);
	//cube.active = false;
	scene.sceneObjects.push_back(cube);
	
	BasicMaterial monkeyMat = BasicMaterial("data/engine/shaders/error/opengl/error");
	monkeyMat.settings.cullFaces = true;
	monkeyMat.settings.cullMode = GL_BACK;
	monkeyMat.setColor(1, 0, 1, 1);
	Renderer monkeyRenderer = Renderer(&monkeyMat, (projectPath + "/Assets/models/suzanne.obj").c_str());
	Transform monkeyTransform = Transform(glm::vec3(0, 3, 0), glm::vec3(0, 0, 0));
	Object monkey = Object("Monkey", &monkeyTransform, &monkeyRenderer);
	//monkey.active = false;
	scene.sceneObjects.push_back(monkey);

	BasicMaterial r2d2Mat = BasicMaterial("data/engine/shaders/default/opengl/default");
	r2d2Mat.settings.cullFaces = true;
	r2d2Mat.settings.cullMode = GL_BACK;
	r2d2Mat.settings.shininess = 20;
	//r2d2Mat.setColor(0, 0.7, 0.1, 1);
	r2d2Mat.setAlbedoMap((projectPath + "/Assets/textures/r2d2_diffuse.png").c_str());
	r2d2Mat.setSpecularMap((projectPath + "/Assets/textures/r2d2_specular.png").c_str());
	r2d2Mat.setNormalMap((projectPath + "/Assets/textures/r2d2_normalmap.png").c_str());
	Renderer r2d2Renderer = Renderer(&r2d2Mat, (projectPath + "/Assets/models/r2d2.obj").c_str());
	Transform r2d2Transform = Transform(glm::vec3(3, 0, 0), glm::vec3(0, 0, 0));
	Object r2d2 = Object("R2D2", &r2d2Transform, &r2d2Renderer);
	r2d2.transform.setScale(glm::vec3(0.3, 0.3, 0.3));
	scene.sceneObjects.push_back(r2d2);

	BasicMaterial sfMat = BasicMaterial("data/engine/shaders/default/opengl/default");
	sfMat.settings.cullFaces = true;
	sfMat.settings.cullMode = GL_BACK;
	sfMat.settings.shininess = 20;
	//r2d2Mat.setColor(0, 0.7, 0.1, 1);
	sfMat.setAlbedoMap((projectPath + "/Assets/textures/spaceship_diffuse.jpg").c_str());
	sfMat.setSpecularMap((projectPath + "/Assets/textures/spaceship_specular.jpg").c_str());
	sfMat.setNormalMap((projectPath + "/Assets/textures/spaceship_normalmap.jpg").c_str());
	Renderer sfRenderer = Renderer(&sfMat, (projectPath + "/Assets/models/spaceship.fbx").c_str());
	Transform sfTransform = Transform(glm::vec3(17, 2, -20), glm::vec3(-80, -30, 0));
	Object sf = Object("Space Ship", &sfTransform, &sfRenderer);
	sf.transform.setScale(glm::vec3(2, 2, 2));
	scene.sceneObjects.push_back(sf);

	BasicMaterial gridMat = BasicMaterial("data/engine/shaders/grid/opengl/grid");
	gridMat.settings.isTransparent = true;
	gridMat.settings.cullMode = GL_BACK;
	gridMat.settings.cullFaces = false;
	Transform gridTransform = Transform(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
	Renderer gridRenderer = Renderer(&gridMat, (projectPath + "/Assets/models/plane.obj").c_str());
	Object grid = Object("Grid", &gridTransform, &gridRenderer);
	grid.transform.setScale(glm::vec3(50, 1, 50));
	scene.sceneObjects.push_back(grid);
	
	scene.sceneCamera.transform.setPosition(glm::vec3(0, 2, 5));
	scene.sceneCamera.transform.setRotation(glm::vec3(15, 0, 0));
}

Editor::Editor(ProjectData projectData)
	: scene("Test scene"){
	// 1. Window + Context creation
    
    WindowManager::InitWindowManager(projectData.graphicsApi);
    WindowManager::CreateEditorWindow(projectData.name);
	
	// 2. Initialize IMGUI
	EditorGui::InitGUI();
	//const PerformanceModule *pModule = new PerformanceModule(std::string("Performance statistics"), &(scene.deltaTime));
	EditorGui::AddWindow(new PerformanceModule(std::string("Performance statistics"), &(scene.deltaTime)));

    // 3. Register assets in file structure

    // 4. Start file watcher

    // 5. Open last scene
	InitScene(projectData.path);
}

void Editor::Run(){
	double lastFrameTime = 0;
	double currentFrameTime = 0;
	// TODO abstract the close condition
    while (!glfwWindowShouldClose(WindowManager::GetWindow_OpenGL())) {
		WindowManager::StartFrame();
		EditorGui::StartFrame();
		
		// ===== Time related stuff
		lastFrameTime = currentFrameTime;
		// TODO abstract the time grabbing
		currentFrameTime = glfwGetTime();
		scene.deltaTime = currentFrameTime - lastFrameTime;

		// Abstract viewport stuff (Context and framebuffer wise)
		glViewport(0, 0, WindowManager::GetWidth(), WindowManager::GetHeight());
		if ((WindowManager::GetWidth() != 0) && (WindowManager::GetHeight() != 0)) {
			scene.sceneCamera.setAspectRatio((float)WindowManager::GetWidth() / WindowManager::GetHeight());
		}

		// Editor Camera updates
		UpdateEditorCamera();
		
		// ===== Animation stuff =====
		scene.sceneObjects.at(2).transform.rotate(glm::vec3(0, scene.deltaTime * 88, 0));
		//scene.sceneObjects.at(3).transform.rotate(glm::vec3(0, delta * 180, 0));

		// ===== Drawing Geometry =====
		// TODO: Draw to a framebuffer
		scene.drawScene();

		// ===== Post processing =====
		// TODO: Implement

		// ===== Gizmo rendering =====
		// TODO: Implement

		// ===== GUI Rendering
		EditorGui::RenderGUI();
		EditorGui::EndFrame();

		// ===== Windowing Stuff =====
		WindowManager::EndFrame();

		// ===== Framerate managing =====
		while (glfwGetTime() < currentFrameTime + (EditorProperties::FrameLimit != -1 ? 1.0 / EditorProperties::FrameLimit : 0)) {
			// TODO: Update this when implementing multi threading
		}
	}
}