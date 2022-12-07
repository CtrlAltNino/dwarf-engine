#pragma once

// system
#include "../../utilities/dpch.h"

// dependencies
#include<glad/glad.h>
//#include<GLFW/glfw3.h>
#include<stb_image.h>
#include<imgui.h>
#include<imgui_impl_sdl.h>
#include<imgui_impl_opengl3.h>
#include<nlohmann/json.hpp>
#include<nfd.h>

#include<SDL2/SDL.h>
#include<SDL2/SDL_opengl.h>

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

/// @brief Struct that contains relevant window information
struct WindowInformation {
	int width;
	int height;
};

/// @brief Main class of the project launcher.
class ProjectLauncher {
private:
	/// @brief Stores the information of the project launcher window
	WindowInformation windowInfo;

	/// @brief ID of the currently selected project in the project list
	int selectedProjectId = -1;

	/// @brief Reference to the SDL internal window
	SDL_Window* window;

	/// @brief OpenGL context tied to the current project launcher window
	SDL_GLContext context;

	/// @brief Current state of the project launcher
	ProjectChooserState state = ProjectChooserState::Choosing;

	/// @brief Font loaded into IMGUI for header text
	ImFont* headerFont;

	/// @brief Font loaded into IMGUI for regular text
	ImFont* textFont;

	/// @brief Loaded image for the github icon
	Texture* githubIcon;

	/// @brief Loaded image for the patreon icon
	Texture* patreonIcon;

	/// @brief Loaded image for the twitter icon
	Texture* twitterIcon;
	
	/// @brief Creates and initializes the window of the project launcher
	int InitWindow();

	/// @brief  Creates and initializes the window of the project launcher
	/// @return Error code
	int InitializeIMGUI();

	/// @brief Renders the project launcher GUI
	void Render();
	
	//void RemoveProjectFromList(int id);
	//void LoadProject(int id);
	//void LoadProjectList();
	//void SaveProjectList();
	//void AddProject();
	//void AddProjectToList(ProjectInformation projectInformation);
	//void CheckProjectListIntegrity();

	/// @brief Renders the project list
	/// @param fWidth Width of the space the project list is occupying in pixel
	/// @param fHeight Height of the space the project list is occupying in pixel
	void RenderProjectList(int fWidth, int fHeight);

	/// @brief Renders the project launcher related buttons (e.g. for creating a new project or settings)
	/// @param fWidth Width of the space the button section is occupying in pixel
	/// @param fHeight Height of the space the button section is occupying in pixel
	void RenderButtons(int fWidth, int fHeight);

	/// @brief Renders the footer information
	/// @param fWidth Width of the space the footer is occupying in pixel
	/// @param fHeight Height of the space the footer is occupying in pixel
	void RenderFooter(int fWidth, int fHeight);

	/// @brief Renders the modal for when the user tries to open a project that could not be found
	void RenderProjectNotFoundModal();

	/// @brief Renders the modal for creating a new project
	void RenderCreateNewProjectModal();

	/// @brief Renders the modal for changing the graphics API of a project
	void RenderChangeGraphicsApiModal();

	//void ChangeGraphicsApi(int id, GraphicsApi api);
	//void SortProjectList();
	//void UpdateSortOrder(int columnId);

	/// @brief Initializes the project launcher
	void InitProjectLauncher();

	//ProjectInformation ExtractProjectInformation(const char* path);
	//int CreateProject(const char* projectName, const char* projectPath, GraphicsApi graphicsApi, ProjectTemplate projectTemplate);
public:
	//const char* graphicsApiNames[4] = { "OpenGL", "Direct3D 11", "Direct3D 12", "Vulkan" };
	
	/// @brief Starts the loop of the project launcher
	/// @return Error code. 0 if successful, error if not.
	int Run();
};