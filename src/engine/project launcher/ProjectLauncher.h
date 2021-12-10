#pragma once

// system
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string>
#include<vector>

// dependencies
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb_image.h>
#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>
#include<nlohmann/json.hpp>
#include<ctime>

#include"../../utilities/FileHandler.h";

enum class ProjectChooserState {Choosing, Done, ProjectNotFound, Canceled};
enum class RenderingApi {OpenGL, DX11, DX12, Vulkan};
enum class ProjectSortOrder {Name, NameReverse, Date, DateReverse};

struct ProjectReturnData {
	std::string name;
	std::string path;
	RenderingApi api;
};

struct ProjectInformation {
	std::string name;
	std::string path;
	int lastOpened;
};

struct WindowInformation {
	int width;
	int height;
};

class ProjectLauncher {
private:
	std::vector<ProjectInformation> projectList;
	WindowInformation windowInfo;
	int selectedProjectId = -1;
	GLFWwindow* window;
	ProjectChooserState state = ProjectChooserState::Choosing;
	ImFont* headerFont;
	ImFont* textFont;
	RenderingApi selectedApi = RenderingApi::OpenGL;
	ProjectSortOrder sortOrder = ProjectSortOrder::Date;
	
	int CreateWindow();
	int InitializeIMGUI();
	void Render();
	void RemoveProjectFromList(int id);
	void LoadProject(int id);
	void LoadProjectList();
	void SaveProjectList();
	void AddProject();
	void CheckProjectListIntegrity();
	void RenderProjectList(int fWidth, int fHeight);
	void RenderButtons(int fWidth, int fHeight);
	void RenderBottomInformation(int fWidth, int fHeight);
	void RenderProjectNotFoundModal();
	void SortProjects();
	void UpdateSortOrder(int columnId);

	
public:
	ProjectLauncher();
	ProjectReturnData Run();
};