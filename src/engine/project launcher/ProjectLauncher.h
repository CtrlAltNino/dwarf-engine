#pragma once

// system
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string>
#include<vector>
#include<thread>

// dependencies
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb_image.h>
#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>
#include<nlohmann/json.hpp>
#include<nfd.h>

#include"../../utilities/Common.h"
#include "../../utilities/FileHandler.h"
#include "../data structures/Texture.h"
#include "ProjectCreator.h"
#include "ProjectListHandler.h"
#include "ProjectSorter.h"
#include "BrowserLinkOpener.h"
#include "TimeUtilities.h"
//#include "../editor/Editor.h"
#include"../editor/EditorController.h"

//enum class ProjectChooserState {Choosing, Done, ProjectNotFound, CreateNewProject, ChangeGraphicsApi, Canceled};
//enum class GraphicsApi {OpenGL, D3D11, D3D12, Vulkan};
//enum class ProjectSortOrder {Name, NameReverse, Date, DateReverse, Api, ApiReverse};
//enum class ProjectTemplate {Blank, Demo1};

struct WindowInformation {
	int width;
	int height;
};

class ProjectLauncher {
private:
	//std::vector<ProjectInformation> projectList;
	WindowInformation windowInfo;
	int selectedProjectId = -1;
	int refreshRate;
	GLFWwindow* window;
	ProjectChooserState state = ProjectChooserState::Choosing;
	ImFont* headerFont;
	ImFont* textFont;
	//std::string defaultProjectPath;
	Texture* githubIcon;
	Texture* patreonIcon;
	Texture* twitterIcon;
	
	int InitWindow();
	int InitializeIMGUI();
	void Render();
	//void RemoveProjectFromList(int id);
	//void LoadProject(int id);
	//void LoadProjectList();
	//void SaveProjectList();
	//void AddProject();
	void AddProjectWrapper();
	void AddProjectToList(ProjectInformation projectInformation);
	void CheckProjectListIntegrity();
	void RenderProjectList(int fWidth, int fHeight);
	void RenderButtons(int fWidth, int fHeight);
	void RenderBottomInformation(int fWidth, int fHeight);
	void RenderProjectNotFoundModal();
	void RenderCreateNewProjectModal();
	void RenderChangeGraphicsApiModal();
	void ChangeGraphicsApi(int id, GraphicsApi api);
	//void SortProjectList();
	//void UpdateSortOrder(int columnId);
	void InitProjectLauncher();
	ProjectInformation ExtractProjectInformation(const char* path);
	//int CreateProject(const char* projectName, const char* projectPath, GraphicsApi graphicsApi, ProjectTemplate projectTemplate);
public:
	//const char* graphicsApiNames[4] = { "OpenGL", "Direct3D 11", "Direct3D 12", "Vulkan" };
	int Run();
};