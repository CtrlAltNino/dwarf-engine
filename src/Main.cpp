// system
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string>

// dependencies
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<stb_image.h>
#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>

// engine related
#include"engine/rendering/BasicMaterial.h"
#include"engine/data structures/Quaternion.h"
#include"engine/scene/Scene.h"
#include"engine/rendering/Shader.h"
#include"engine/data structures/VAO.h"
#include"engine/data structures/VBO.h"
#include"engine/data structures/EBO.h"
#include"engine/editor/EditorProperties.h"
#include"engine/project launcher/ProjectLauncher.h"

// macros

// global variables
Scene scene = Scene("Test Scene");
EditorProperties editorProperties = EditorProperties();
static int menuBarHeight;
static float menuPercent;
std::string projectPath = "project/";
glm::vec4 fogColor = glm::vec4(0.1, 0.1, 0.1, 1);
float fogStart = 10;
float fogEnd = 60;



void window_focus_callback(GLFWwindow* window, int focused)
{
	if (focused)
	{
		// The window gained input focus
		//std::cout << "Focused" << std::endl;
		double lastCursorXPos, lastCursorYPos;
		glfwGetCursorPos(window, &lastCursorXPos, &lastCursorYPos);
		scene.lastMousePos = glm::vec2(lastCursorXPos, lastCursorYPos);
	}
	else
	{
		// The window lost input focus
		// std::cout << "De focused" << std::endl;
	}
}

void InitScene(Scene* scene) {
	Scene& testScene = *(Scene*)scene;
	// Creating Objects
	BasicMaterial cubeMat = BasicMaterial("data/engine/shaders/default");
	cubeMat.setAlbedoMap((projectPath + "/Assets/textures/popcat.jpg").c_str());
	cubeMat.settings.cullMode = GL_BACK;
	cubeMat.settings.cullFaces = true;
	
	Transform cubeTransform = Transform(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0));
	Renderer cubeRenderer = Renderer(&cubeMat, (projectPath + "/Assets/models/cube.obj").c_str());
	Object cube = Object("Popcat cube", &cubeTransform, &cubeRenderer);
	//cube.active = false;
	testScene.sceneObjects.push_back(cube);
	
	BasicMaterial monkeyMat = BasicMaterial("data/engine/shaders/default");
	monkeyMat.settings.cullFaces = true;
	monkeyMat.settings.cullMode = GL_BACK;
	monkeyMat.setColor(1, 0, 1, 1);
	Renderer monkeyRenderer = Renderer(&monkeyMat, (projectPath + "/Assets/models/suzanne.obj").c_str());
	Transform monkeyTransform = Transform(glm::vec3(0, 3, 0), glm::vec3(0, 0, 0));
	Object monkey = Object("Monkey", &monkeyTransform, &monkeyRenderer);
	//monkey.active = false;
	testScene.sceneObjects.push_back(monkey);

	BasicMaterial r2d2Mat = BasicMaterial("data/engine/shaders/default");
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
	testScene.sceneObjects.push_back(r2d2);

	BasicMaterial sfMat = BasicMaterial("data/engine/shaders/default");
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
	testScene.sceneObjects.push_back(sf);

	BasicMaterial gridMat = BasicMaterial("data/engine/shaders/grid");
	gridMat.settings.isTransparent = true;
	gridMat.settings.cullMode = GL_BACK;
	gridMat.settings.cullFaces = false;
	Transform gridTransform = Transform(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
	Renderer gridRenderer = Renderer(&gridMat, (projectPath + "/Assets/models/plane.obj").c_str());
	Object grid = Object("Grid", &gridTransform, &gridRenderer);
	grid.transform.setScale(glm::vec3(50, 1, 50));
	testScene.sceneObjects.push_back(grid);
	
	testScene.sceneCamera.transform.setPosition(glm::vec3(0, 2, 5));
	testScene.sceneCamera.transform.setRotation(glm::vec3(15, 0, 0));
}

int windowState = 2;
int windowXPos = 0;
int windowYPos = 0;
int windowWidth = 0;
int windowHeight = 0;
int wasPreviouslyMaximized = 0;

void fullscreen(GLFWwindow* window, GLFWmonitor* _monitor, const GLFWvidmode* mode) {
	if (windowState == 2) {
		wasPreviouslyMaximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
		glfwGetWindowPos(window, &windowXPos, &windowYPos);
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
	}
	//glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
	glfwSetWindowMonitor(window, _monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
}

void windowed(GLFWwindow* window) {
	//glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
	glfwSetWindowMonitor(window, nullptr, windowXPos, windowYPos, windowWidth, windowHeight, NULL);
	if (wasPreviouslyMaximized) {
		//std::cout << "maximizing" << std::endl;
		glfwMaximizeWindow(window);
	}
}

void borderlessFullscreen(GLFWwindow* window, const GLFWvidmode* mode) {
	if (windowState == 2) {
		wasPreviouslyMaximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
		glfwGetWindowPos(window, &windowXPos, &windowYPos);
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
	}
	
	/*int maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
	if (maximized) {
		glfwRestoreWindow(window);
	}*/
	//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
	glfwSetWindowMonitor(window, nullptr, 0, 0, mode->width, mode->height, NULL);
}

static void ShowExampleMenuFile()
{
	//IMGUI_DEMO_MARKER("Examples/Menu");
	ImGui::MenuItem("(demo menu)", NULL, false, false);
	if (ImGui::MenuItem("New")) {}
	if (ImGui::MenuItem("Open", "Ctrl+O")) {}
	if (ImGui::BeginMenu("Open Recent"))
	{
		ImGui::MenuItem("fish_hat.c");
		ImGui::MenuItem("fish_hat.inl");
		ImGui::MenuItem("fish_hat.h");
		if (ImGui::BeginMenu("More.."))
		{
			ImGui::MenuItem("Hello");
			ImGui::MenuItem("Sailor");
			if (ImGui::BeginMenu("Recurse.."))
			{
				ShowExampleMenuFile();
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save", "Ctrl+S")) {}
	if (ImGui::MenuItem("Save As..")) {}

	ImGui::Separator();
	//IMGUI_DEMO_MARKER("Examples/Menu/Options");
	if (ImGui::BeginMenu("Options"))
	{
		static bool enabled = true;
		ImGui::MenuItem("Enabled", "", &enabled);
		ImGui::BeginChild("child", ImVec2(0, 60), true);
		for (int i = 0; i < 10; i++)
			ImGui::Text("Scrolling Text %d", i);
		ImGui::EndChild();
		static float f = 0.5f;
		static int n = 0;
		ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
		ImGui::InputFloat("Input", &f, 0.1f);
		ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
		ImGui::EndMenu();
	}

	//IMGUI_DEMO_MARKER("Examples/Menu/Colors");
	if (ImGui::BeginMenu("Colors"))
	{
		float sz = ImGui::GetTextLineHeight();
		for (int i = 0; i < ImGuiCol_COUNT; i++)
		{
			const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
			ImGui::Dummy(ImVec2(sz, sz));
			ImGui::SameLine();
			ImGui::MenuItem(name);
		}
		ImGui::EndMenu();
	}

	// Here we demonstrate appending again to the "Options" menu (which we already created above)
	// Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
	// In a real code-base using it would make senses to use this feature from very different code locations.
	if (ImGui::BeginMenu("Options")) // <-- Append!
	{
		//IMGUI_DEMO_MARKER("Examples/Menu/Append to an existing menu");
		static bool b = true;
		ImGui::Checkbox("SomeOption", &b);
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Disabled", false)) // Disabled
	{
		IM_ASSERT(0);
	}
	if (ImGui::MenuItem("Checked", NULL, true)) {}
	if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}

static void DrawMainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		ImVec2 size = ImGui::GetWindowSize();
		menuBarHeight = size.y;
		if (ImGui::BeginMenu("File"))
		{
			ShowExampleMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void DrawEditorSettings(GLFWwindow* window, GLFWmonitor* _monitor, const GLFWvidmode* mode) {
	ImGuiWindowFlags window_flags = 0;

	window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	//window_flags |= ImGuiWindowFlags_MenuBar;
	// Main body of the Demo window starts here.

	int fWidth, fHeight;
	
	glfwGetFramebufferSize(window, &fWidth, &fHeight);


	ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight));
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(400, fHeight-menuBarHeight));
	//ImGui::PushStyleVar(ImGuiStyleVar_Window)
	ImGui::SetNextWindowSize(ImVec2(menuPercent * fWidth, fHeight - menuBarHeight));

	//ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 minSize = ImVec2(400, fHeight - menuBarHeight);
	ImVec2 maxSize = ImVec2(fWidth, fHeight - menuBarHeight);
	//ImGui::SetWindowSize(ImVec2(windowSize.x, fHeight - menuBarHeight));
	ImGui::SetNextWindowSizeConstraints(minSize, maxSize);
	if (!ImGui::Begin("Editor Settings", NULL, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	static float fov = scene.sceneCamera.getFov();
	ImGui::Text("Hello, world!");
	ImGui::SliderFloat("Field of view", &fov, 45.0f, 105.0f);
	ImGui::SliderFloat("Mouse sensitivity", &editorProperties.cameraRotationSpeed, 0.05, 0.3);
	scene.sceneCamera.setFov(fov);
	//ImGui::Text(std::string("ImGui fps: " + std::to_string(io.Framerate)).c_str());
	ImGui::Text(std::string("glfwGetTime() fps: " + std::to_string(1.0 / scene.deltaTime)).c_str());
	const char* items[] = { "30", "60", "75", "120", "144", "Uncapped" };
	static int item_current = 4;
	ImGui::Combo("FPS Cap", &item_current, items, IM_ARRAYSIZE(items));

	switch (item_current) {
	case 0: editorProperties.FrameLimit = 30;
		break;
	case 1: editorProperties.FrameLimit = 60;
		break;
	case 2: editorProperties.FrameLimit = 75;
		break;
	case 3: editorProperties.FrameLimit = 120;
		break;
	case 4: editorProperties.FrameLimit = 144;
		break;
	case 5: editorProperties.FrameLimit = -1;
		break;
	default: editorProperties.FrameLimit = 60;
	}
	//ImGui::BeginCombo("Frame Cap", "60");
	//ImGui::Combo("30", 0,);
	//ImGui::EndCombo();
	//ImGui::ShowDemoWindow();

	const char* windowModeItems[] = { "Borderless Fullscreen", "Fullscreen", "Windowed" };
	static int currentWindowMode = 2;
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.62f);
	ImGui::Combo("Window Mode", &currentWindowMode, windowModeItems, IM_ARRAYSIZE(windowModeItems));
	ImGui::SameLine();
	if (ImGui::Button("Apply")) {
		switch (currentWindowMode) {
		case 0:
			if (windowState != currentWindowMode) {
				borderlessFullscreen(window, mode);
				windowState = currentWindowMode;
			}
			break;
		case 1:
			if (windowState != currentWindowMode) {
				fullscreen(window, _monitor, mode);
				windowState = currentWindowMode;
			}
			break;
		case 2:
			if (windowState != currentWindowMode) {
				windowed(window);
				windowState = currentWindowMode;
			}
			break;
		}
	}

	static bool vsync = false;

	ImGui::Checkbox("Vertical Synchronisation", &vsync);

	ImGui::ColorPicker3("Fog Color", &fogColor[0]);
	ImGui::SliderFloat("Fog Start Distance", &fogStart, 0, 100);
	ImGui::SliderFloat("Fog End Distance", &fogEnd, 0, 100);

	for (int i = 0; i < scene.sceneObjects.size(); i++) {
		Object* currObj = &scene.sceneObjects[i];
		currObj->renderer.getMaterial()->settings.fogColor = fogColor;
		currObj->renderer.getMaterial()->settings.fogStart = fogStart;
		currObj->renderer.getMaterial()->settings.fogEnd = fogEnd;
	}
	
	
	glfwSwapInterval(vsync ? 1 : 0);
	menuPercent = ImGui::GetWindowSize().x / fWidth;

	ImGui::End();

	//ImGui::PopStyleVar();
}

int OpenProjectInOpenGL(std::string path) {
	projectPath = path;
	glfwInit();
	// get resolution of monitor
	GLFWmonitor* _monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(_monitor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "Simple 3D Engine", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeLimits(window, 1280, 720, GLFW_DONT_CARE, GLFW_DONT_CARE);

	GLFWimage icon[1];
	int numColChannel;
	icon[0].pixels = stbi_load("data/engine/img/icons/icon.png", &icon[0].width, &icon[0].height, &numColChannel, 0); //rgba channels
	glfwSetWindowIcon(window, 1, icon);
	stbi_image_free(icon[0].pixels);
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwSetWindowFocusCallback(window, window_focus_callback);
	glViewport(0, 0, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);

	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init("#version 130");

	io.ConfigWindowsResizeFromEdges = false;

	double lastTime = glfwGetTime();
	double cooldown = 0.5f;
	double counter = cooldown;
	glfwSwapInterval(0);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(window);
	int width, height;
	float moveSpeed = 4;
	float rotateSpeed = 95;
	float cameraMovementSpeed = 4;

	InitScene(&scene);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double currentTime = glfwGetTime();
		scene.deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//ImGui::ShowDemoWindow();
		DrawMainMenu();
		DrawEditorSettings(window, _monitor, mode);

		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);
		if ((width != 0) && (height != 0)) {
			scene.sceneCamera.setAspectRatio((float)width / height);
		}


		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		if (state == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			double currentCursorXPos, currentCursorYPos;
			glfwGetCursorPos(window, &currentCursorXPos, &currentCursorYPos);

			if (scene.lastMousePos != glm::vec2(-1)) {
				scene.deltaMousePos = glm::vec2(currentCursorXPos - scene.lastMousePos.x, currentCursorYPos - scene.lastMousePos.y);
			}
			else {
				scene.deltaMousePos = glm::vec2(0);
			}

			scene.lastMousePos = glm::vec2(currentCursorXPos, currentCursorYPos);

			if (scene.deltaMousePos.length() > 0) {
				float crsSquared = editorProperties.cameraRotationSpeed * editorProperties.cameraRotationSpeed;
				float yAngle = scene.deltaMousePos.x * crsSquared;
				float xAngle = scene.deltaMousePos.y * crsSquared;
				scene.sceneCamera.transform.rotate(glm::vec3(0, 1, 0), yAngle);
				scene.sceneCamera.transform.rotate(scene.sceneCamera.transform.getRotation() * glm::vec3(1, 0, 0), xAngle);
			}

		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			scene.deltaMousePos = glm::vec2(0);
			scene.lastMousePos = glm::vec2(-1);
		}

		int stateW = glfwGetKey(window, GLFW_KEY_W);
		int stateA = glfwGetKey(window, GLFW_KEY_A);
		int stateS = glfwGetKey(window, GLFW_KEY_S);
		int stateD = glfwGetKey(window, GLFW_KEY_D);
		int stateE = glfwGetKey(window, GLFW_KEY_E);
		int stateQ = glfwGetKey(window, GLFW_KEY_Q);
		int stateLeftShift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
		glm::vec3 movementVector = { ((stateA == GLFW_PRESS) ? -1 : 0) + ((stateD == GLFW_PRESS) ? 1 : 0),
			((stateQ == GLFW_PRESS) ? -1 : 0) + ((stateE == GLFW_PRESS) ? 1 : 0),
			((stateW == GLFW_PRESS) ? -1 : 0) + ((stateS == GLFW_PRESS) ? 1 : 0) };


		if (glm::length(movementVector) > 0) {
			movementVector = glm::normalize(movementVector);
			movementVector *= scene.deltaTime * moveSpeed * ((stateLeftShift == GLFW_PRESS) ? 2 : 1);
		}

		scene.sceneCamera.transform.translate(scene.sceneCamera.transform.getRotation() * movementVector);
		scene.sceneObjects.at(2).transform.rotate(glm::vec3(0, scene.deltaTime * 88, 0));
		//scene.sceneObjects.at(3).transform.rotate(glm::vec3(0, delta * 180, 0));
		scene.drawScene();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		while (glfwGetTime() < currentTime + (editorProperties.FrameLimit != -1 ? 1.0 / editorProperties.FrameLimit : 0)) {
			// TODO: Update this when implementing multi threading
		}
	}

	/*VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	VAO2.Delete();
	VBO2.Delete();
	EBO2.Delete();
	gridShaderProgram.Delete();
	glDeleteTextures(1, &texture);*/

	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

int OpenProject(std::string path, RenderingApi api) {
	int returnCode = 0;
	
	switch (api) {
	case RenderingApi::OpenGL:
		returnCode = OpenProjectInOpenGL(path);
		break;
	case RenderingApi::DirectX11:
		std::cout << "Couldn't open project: DirectX 11 is not yet supported!";
		break;
	case RenderingApi::DirectX12:
		std::cout << "Couldn't open project: DirectX 12 is not yet supported!";
		break;
	case RenderingApi::Vulkan:
		std::cout << "Couldn't open project: Vulkan is not yet supported!";
		break;
	}

	return returnCode;
}

int main() {
	int returnCode = 0;
	ProjectLauncher chooser = ProjectLauncher();
	ProjectReturnData projectData = chooser.Run();

	if ((projectData.name != "") && (projectData.path != "")) {
		returnCode = OpenProject(projectData.path, projectData.renderingApi);
	}
	else {
		//std::cout << "Not opening any project!" << std::endl;
	}
	
	return returnCode;
}