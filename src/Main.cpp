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
#include"engine/data/Quaternion.h"
#include"engine/scene/Scene.h"
#include"engine/rendering/Shader.h"
#include"engine/data/VAO.h"
#include"engine/data/VBO.h"
#include"engine/data/EBO.h"

// macros
#define INITIAL_WINDOW_WIDTH (1280)
#define INITIAL_WINDOW_HEIGHT (720)

// global variables
Scene scene = Scene("Test Scene");

GLfloat verticesTriangle[] =
{
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
	0.5, -0.5f * float(sqrt(3)) / 3, 0.0f,
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f
};

GLuint indicesTriangle[] =
{
	0, 1, 2
};

GLfloat verticesQuad[] =
{
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.0f, 0.0f, 0.0f
};

GLuint indicesQuad[] =
{
	0, 1, 3,
	3, 1, 2
};

GLfloat verticesGrid[] =
{
	-1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
	-1.0f, 0.0f, -1.0f, 0.0f, 0.0f
};

GLuint indicesGrid[] =
{
	0, 1, 3,
	3, 1, 2
};

GLfloat verticesCube[] =
{
	// Position           // UV Coordinates
	-0.5f, -0.5f, 0.5f,   0.0f, 1.0f,   // 0 Vorne links unten
	0.5f, -0.5f, 0.5f,    1.0f, 1.0f,    // 1 Vorne rechts unten
	0.5f, 0.5f, 0.5f,     1.0f, 0.0f,     // 2 Vorne rechts oben
	-0.5f, 0.5f, 0.5f,    0.0f, 0.0f,    // 3 Vorne links oben
	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  // 4 Hinten links unten
	0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   // 5 Hinten rechts unten
	0.5f, 0.5f, -0.5f,    0.0f, 0.0f,     // 6 Hinten rechts oben
	-0.5f, 0.5f, -0.5f,   1.0f, 0.0f   // 7 Hinten links oben
};

GLuint indicesCube[] =
{
	0, 1, 3, // Vorne links
	3, 1, 2, // Vorne rechts
	
	4, 0, 7, // Links links
	7, 0, 3, // Links rechts
	
	1, 5, 2, // Rechts links
	2, 5, 6, // Rechts rechts
	
	3, 2, 7, // Oben links
	7, 2, 6, // Oben rechts
	
	4, 5, 0, // Unten links
	0, 5, 1, // Unten rechts
	
	5, 4, 6, // Hinten links
	6, 4, 7, // Hinten rechts
};

std::string readFile(const char* filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void window_focus_callback(GLFWwindow* window, int focused)
{
	if (focused)
	{
		// The window gained input focus
		std::cout << "Focused" << std::endl;
		double lastCursorXPos, lastCursorYPos;
		glfwGetCursorPos(window, &lastCursorXPos, &lastCursorYPos);
		scene.lastMousePos = glm::vec2(lastCursorXPos, lastCursorYPos);
	}
	else
	{
		// The window lost input focus
		std::cout << "De focused" << std::endl;
	}
}

void InitScene(Scene* scene) {
	Scene& testScene = *(Scene*)scene;
	// Creating Objects
	BasicMaterial cubeMat = BasicMaterial("default");
	cubeMat.setAlbedoMap("data/textures/popcat.jpg");
	cubeMat.settings.cullMode = GL_BACK;
	cubeMat.settings.cullFaces = true;
	
	Transform cubeTransform = Transform(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0));
	Renderer cubeRenderer = Renderer(&cubeMat, "data/models/cube3.obj");
	Object cube = Object("Popcat cube", &cubeTransform, &cubeRenderer);
	//cube.active = false;
	testScene.sceneObjects.push_back(cube);
	
	BasicMaterial monkeyMat = BasicMaterial("default");
	monkeyMat.settings.cullFaces = true;
	monkeyMat.settings.cullMode = GL_BACK;
	monkeyMat.setColor(1, 0, 1, 1);
	Renderer monkeyRenderer = Renderer(&monkeyMat, "data/models/monkey2.obj");
	Transform monkeyTransform = Transform(glm::vec3(0, 3, 0), glm::vec3(0, 0, 0));
	Object monkey = Object("Monkey", &monkeyTransform, &monkeyRenderer);
	//monkey.active = false;
	testScene.sceneObjects.push_back(monkey);

	BasicMaterial r2d2Mat = BasicMaterial("default");
	r2d2Mat.settings.cullFaces = true;
	r2d2Mat.settings.cullMode = GL_BACK;
	r2d2Mat.settings.shininess = 20;
	//r2d2Mat.setColor(0, 0.7, 0.1, 1);
	r2d2Mat.setAlbedoMap("data/models/r2d2/R2_diffuse.png");
	r2d2Mat.setSpecularMap("data/models/r2d2/R2_spec.png");
	r2d2Mat.setNormalMap("data/models/r2d2/NormalMap.png");
	Renderer r2d2Renderer = Renderer(&r2d2Mat, "data/models/r2d2/Low_Poly_R2D2.obj");
	Transform r2d2Transform = Transform(glm::vec3(3, 0, 0), glm::vec3(0, 0, 0));
	Object r2d2 = Object("R2D2", &r2d2Transform, &r2d2Renderer);
	r2d2.transform.setScale(glm::vec3(0.3, 0.3, 0.3));
	testScene.sceneObjects.push_back(r2d2);

	BasicMaterial sfMat = BasicMaterial("default");
	sfMat.settings.cullFaces = true;
	sfMat.settings.cullMode = GL_BACK;
	sfMat.settings.shininess = 20;
	//r2d2Mat.setColor(0, 0.7, 0.1, 1);
	sfMat.setAlbedoMap("data/models/SF_Fighter/SF_Fighter-Albedo.jpg");
	sfMat.setSpecularMap("data/models/SF_Fighter/SF_Fighter-Specular.jpg");
	sfMat.setNormalMap("data/models/SF_Fighter/SF_Fighter-Normal.jpg");
	Renderer sfRenderer = Renderer(&sfMat, "data/models/SF_Fighter/SciFi_Fighter.fbx");
	Transform sfTransform = Transform(glm::vec3(17, 2, -20), glm::vec3(-80, -30, 0));
	Object sf = Object("Space Ship", &sfTransform, &sfRenderer);
	sf.transform.setScale(glm::vec3(2, 2, 2));
	testScene.sceneObjects.push_back(sf);

	BasicMaterial gridMat = BasicMaterial("grid");
	gridMat.settings.isTransparent = true;
	gridMat.settings.cullMode = GL_BACK;
	gridMat.settings.cullFaces = false;
	Transform gridTransform = Transform(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
	Renderer gridRenderer = Renderer(&gridMat, "data/models/plane.obj");
	Object grid = Object("Grid", &gridTransform, &gridRenderer);
	grid.transform.setScale(glm::vec3(50, 1, 50));
	testScene.sceneObjects.push_back(grid);
	
	testScene.sceneCamera.transform.setPosition(glm::vec3(0, 2, 5));
	testScene.sceneCamera.transform.setRotation(glm::vec3(15, 0, 0));
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "S3DE", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	GLFWimage icon[1];
	int numColChannel;
	icon[0].pixels = stbi_load("icon.png", &icon[0].width, &icon[0].height, &numColChannel, 0); //rgba channels 
	std::cout << "height: " << icon->width << " | width: " << icon->height << std::endl;
	glfwSetWindowIcon(window, 1, icon);
	stbi_image_free(icon[0].pixels);
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwSetWindowFocusCallback(window, window_focus_callback);
	glViewport(0, 0, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
	
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init("#version 130");
	
	

	double lastTime = glfwGetTime();
	double cooldown = 0.5f;
	double counter = cooldown;
	glfwSwapInterval(0);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwMaximizeWindow(window);
	glfwSwapBuffers(window);
	int width, height;
	float moveSpeed = 4;
	float rotateSpeed = 95;
	float cameraRotationSpeed = 75;
	float cameraMovementSpeed = 4;

	InitScene(&scene);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			static float f = 0.0f;
			static int counter = 0;
			static float fov = scene.sceneCamera.getFov();
			ImGui::Text("Hello, world!");
			ImGui::SliderFloat("FOV", &fov, 15.0f, 105.0f);
			scene.sceneCamera.setFov(fov);
			ImGui::ShowDemoWindow();
		}

		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);
		if ((width != 0) && (height != 0)) {
			scene.sceneCamera.setAspectRatio((float)width / height);
		}
		
		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;
		
		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		if (state == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			scene.sceneCamera.transform.rotate(glm::vec3(0, 1, 0), scene.deltaMousePos.x * (float)delta * cameraRotationSpeed);
			scene.sceneCamera.transform.rotate(scene.sceneCamera.transform.getRotation() * glm::vec3(1, 0, 0), scene.deltaMousePos.y * (float)delta * cameraRotationSpeed);

			double currentCursorXPos, currentCursorYPos;
			glfwGetCursorPos(window, &currentCursorXPos, &currentCursorYPos);

			if (scene.lastMousePos != glm::vec2(-1)) {
				scene.deltaMousePos = glm::vec2(currentCursorXPos - scene.lastMousePos.x, currentCursorYPos - scene.lastMousePos.y);
			}
			else {
				scene.deltaMousePos = glm::vec2(0);
			}

			scene.lastMousePos = glm::vec2(currentCursorXPos, currentCursorYPos);
			
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
		glm::vec3 movementVector = {((stateA == GLFW_PRESS) ? -1 : 0) + ((stateD == GLFW_PRESS) ? 1 : 0),
			((stateQ == GLFW_PRESS) ? -1 : 0) + ((stateE == GLFW_PRESS) ? 1 : 0),
			((stateW == GLFW_PRESS) ? -1 : 0) + ((stateS == GLFW_PRESS) ? 1 : 0) };
		
		
		if (glm::length(movementVector) > 0) {
			movementVector = glm::normalize(movementVector);
			movementVector *= (float)delta * moveSpeed * ((stateLeftShift == GLFW_PRESS) ? 2 : 1);
		}
		
		scene.sceneCamera.transform.translate(scene.sceneCamera.transform.getRotation() * movementVector);
		scene.sceneObjects.at(2).transform.rotate(glm::vec3(0, delta * 88, 0));
		//scene.sceneObjects.at(3).transform.rotate(glm::vec3(0, delta * 180, 0));
		scene.drawScene();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
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