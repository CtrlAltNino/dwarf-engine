#include "ProjectLauncher.h"
#define INITIAL_WINDOW_WIDTH (960)
#define INITIAL_WINDOW_HEIGHT (540)
#define PROJECT_BUTTON_WINDOW_WIDTH (200)
#define PROJECT_INFORMATION_HEIGHT (30)

std::string GetSettingsPath() {
	std::string savedProjectsPath = "";
	static char str[128];
	
	// Get Documents directory path
	{
		PWSTR path = NULL;
		HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path);

		if (SUCCEEDED(hr)) {
			size_t i;
			wcstombs_s(&i, str, (size_t)128, path, (size_t)127);
			savedProjectsPath = std::string((const char*)str);
		}

		CoTaskMemFree(path);
	}
	
	if (savedProjectsPath != "") {
		size_t pos;
		while ((pos = savedProjectsPath.find('\\')) != std::string::npos) {
			savedProjectsPath.replace(pos, 1, "/");
		}
		savedProjectsPath += "/Simple 3D Engine/settings/";
	}
	
	//return "C:/Users/ninom/Documents/yeet";
	return savedProjectsPath;
}

ProjectReturnData ProjectLauncher::Run() {
	glfwInit();
	
	if (window == nullptr) {
		InitWindow();
	}

	InitProjectLauncher();
	InitializeIMGUI();

	glfwShowWindow(window);

	while (((state != ProjectChooserState::Done) && (state != ProjectChooserState::Canceled)) && !glfwWindowShouldClose(window)) {
		glfwPollEvents();
		
		glfwGetWindowSize(window, &windowInfo.width, &windowInfo.height);
		glViewport(0, 0, windowInfo.width, windowInfo.height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		Render();
		
		glfwSwapBuffers(window);

		//while (glfwGetTime() < currentTime + (editorProperties.FrameLimit != -1 ? 1.0 / editorProperties.FrameLimit : 0)) {
			// TODO: Update this when implementing multi threading
		//}
	}

	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();

	ProjectReturnData projectData;

	if ((state == ProjectChooserState::Done) && !projectList.empty() && projectList.size() > selectedProjectId) {
		projectData.name = projectList[selectedProjectId].name;
		projectData.path = projectList[selectedProjectId].path;
		projectData.graphicsApi = (GraphicsApi)projectList[selectedProjectId].graphicsApi;
		projectList[selectedProjectId].lastOpened = time(0);
		SaveProjectList();
	}
	
	return projectData;
}

void ProjectLauncher::InitProjectLauncher() {
	//defaultProjectPath
	LoadProjectList();
	githubIcon = new Texture("data/engine/img/icons/github.png", GL_LINEAR, GL_REPEAT, GL_RGBA, GL_UNSIGNED_BYTE);
	patreonIcon = new Texture("data/engine/img/icons/patreon.png", GL_LINEAR, GL_REPEAT, GL_RGBA, GL_UNSIGNED_BYTE);
	twitterIcon = new Texture("data/engine/img/icons/twitter.png", GL_LINEAR, GL_REPEAT, GL_RGBA, GL_UNSIGNED_BYTE);

	static char str[128];
	{
		PWSTR path = NULL;
		HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path);

		if (SUCCEEDED(hr)) {
			//newProjectPath = (const char*)path;
			size_t i;
			wcstombs_s(&i, str, (size_t)128, path, (size_t)127);
			defaultProjectPath = std::string((const char*)str);
		}

		CoTaskMemFree(path);
	}
}

int ProjectLauncher::InitWindow() {
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
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	
	window = glfwCreateWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "Project Launcher", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetWindowPos(window, mode->width/2 - (INITIAL_WINDOW_WIDTH / 2), mode->height / 2 - (INITIAL_WINDOW_HEIGHT / 2));
	glfwSetWindowSizeLimits(window, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);

	GLFWimage icon[1];
	int numColChannel;
	icon[0].pixels = stbi_load("data/engine/img/icons/icon.png", &icon[0].width, &icon[0].height, &numColChannel, 0); //rgba channels
	glfwSetWindowIcon(window, 1, icon);
	stbi_image_free(icon[0].pixels);
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);

	return 0;
}

int ProjectLauncher::InitializeIMGUI() {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init("#version 130");
	io.Fonts->AddFontDefault();
	headerFont = io.Fonts->AddFontFromFileTTF("data\\engine\\fonts\\Roboto-Light.ttf", 26);
	textFont = io.Fonts->AddFontFromFileTTF("data\\engine\\fonts\\Roboto-Regular.ttf", 15);

	return 0;
}

void ProjectLauncher::Render() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	int fWidth, fHeight;
	glfwGetFramebufferSize(window, &fWidth, &fHeight);

	RenderProjectList(fWidth, fHeight);
	RenderButtons(fWidth, fHeight);
	RenderBottomInformation(fWidth, fHeight);

	if (state == ProjectChooserState::ProjectNotFound) {
		state = ProjectChooserState::Choosing;
		ImGui::OpenPopup("Project not found!");
	}
	else if (state == ProjectChooserState::CreateNewProject) {
		state = ProjectChooserState::Choosing;
		ImGui::OpenPopup("Create new project");
	}
	else if (state == ProjectChooserState::ChangeGraphicsApi) {
		state = ProjectChooserState::Choosing;
		ImGui::OpenPopup("Change Graphics API");
	}

	RenderProjectNotFoundModal();
	RenderCreateNewProjectModal();
	RenderChangeGraphicsApiModal();

	//if (state == ProjectChooserState::ProjectNotFound) {
	//	RenderProjectNotFoundModal();
	//}

	//ImGui::ShowDemoWindow();
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ProjectLauncher::RenderProjectList(int fWidth, int fHeight) {
	static bool* rowSelected = new bool[projectList.size()];
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	//window_flags |= ImGuiWindowFlags_MenuBar;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(60, 8));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(fWidth - PROJECT_BUTTON_WINDOW_WIDTH, fHeight - PROJECT_INFORMATION_HEIGHT));
	//ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(59 / 255.0, 66 / 255.0, 82 / 255.0, 1.0));
	ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(46, 52, 64, 255));

	if (!ImGui::Begin("Project List", NULL, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
	ImGui::PushFont(headerFont);
	ImGui::Text("Your Projects");
	ImGui::PopFont();
	ImGui::Separator();
	ImGui::PopStyleVar(1);


	const int COLUMNS_COUNT = 4;
	const float ROW_HEIGHT = 50;
	const float HEADER_ROW_HEIGHT = 40;
	ImVec2 cellPadding = ImVec2(10.0f, 10.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cellPadding);
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10);
	ImGui::PushStyleColor(ImGuiCol_TableBorderLight, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, IM_COL32(59, 66, 82, 255));
	// /*ImGuiTableFlags_Borders | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable |*/ ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_RowBg
	if (ImGui::BeginTable("Project entries", COLUMNS_COUNT, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_PreciseWidths))
	{
		float initialWidth = ImGui::GetContentRegionAvailWidth() - 80;
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder | ImGuiTableColumnFlags_WidthFixed, initialWidth * 0.2);
		ImGui::TableSetupColumn("Path", ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder | ImGuiTableColumnFlags_WidthFixed, initialWidth * 0.4);
		ImGui::TableSetupColumn("Last opened", ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder | ImGuiTableColumnFlags_WidthFixed, initialWidth * 0.25);
		ImGui::TableSetupColumn("API", ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder | ImGuiTableColumnFlags_WidthFixed, initialWidth * 0.15);
		//ImGui::Columns(4);
		//ImGui::SetColumnWidth(-1, 50);

		// Dummy entire-column selection storage
		// FIXME: It would be nice to actually demonstrate full-featured selection using those checkbox.
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		std::fill(rowSelected, rowSelected + projectList.size(), false);

		// Instead of calling TableHeadersRow() we'll submit custom headers ourselves
		ImGui::TableNextRow(ImGuiTableRowFlags_Headers, HEADER_ROW_HEIGHT);
		ImGui::PushFont(headerFont);
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(76, 86, 106, 255));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, IM_COL32(129, 161, 193, 255));
		/*ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 10);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 10);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);
		ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 10);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10);*/
		for (int column = 0; column < COLUMNS_COUNT; column++)
		{
			ImGui::TableSetColumnIndex(column);
			const char* column_name = ImGui::TableGetColumnName(column); // Retrieve name passed to TableSetupColumn()
			ImGui::PushID(column);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (HEADER_ROW_HEIGHT / 2 - cellPadding.y / 2) - ImGui::GetFontSize() / 2);
			ImGui::TableHeader(column_name);

			if (ImGui::IsItemHovered()) {
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}

			if (ImGui::IsItemClicked()) {
				UpdateSortOrder(column);
			}

			ImGui::PopID();
		}
		ImGui::PopStyleColor(2);
		//ImGui::PopStyleVar(6);
		ImGui::PopFont();

		//ImGui::Separator();
		ImGui::PushFont(textFont);
		//for (int row = 0; row < projectList.size(); row += (deleted ? 0 : 1))
		for (int row = 0; row < projectList.size(); row++)
		{
			ImGui::TableNextRow(ImGuiTableRowFlags_None, ROW_HEIGHT);
			for (int column = 0; (column < COLUMNS_COUNT) && (row < projectList.size()); column++)
			{
				ImGui::TableSetColumnIndex(column);
				std::string cellText = "";
				switch (column) {
				case 0:
					cellText = projectList[row].name.c_str();
					break;
				case 1:
					cellText = projectList[row].path.c_str();
					break;
				case 2:
				{
					time_t lastOpenedTime = projectList[row].lastOpened;
					if (lastOpenedTime != -1) {
						time_t currentTime = time(0);
						struct tm ct;
						struct tm lot;
						localtime_s(&ct, &currentTime);
						localtime_s(&lot, &lastOpenedTime);
						if (ct.tm_year != lot.tm_year) {
							int diff = ct.tm_year - lot.tm_year;
							if (diff == 1) {
								cellText = "a year ago";
							}
							else {
								cellText = std::to_string(diff) + " years ago";
							}
						}
						else if (ct.tm_mon != lot.tm_mon) {
							int diff = ct.tm_mon - lot.tm_mon;
							if (diff == 1) {
								cellText = "a month ago";
							}
							else {
								cellText = std::to_string(diff) + " months ago";
							}
						}
						else if (ct.tm_mday != lot.tm_mday) {
							int diff = ct.tm_mday - lot.tm_mday;
							if (diff == 1) {
								cellText = "a day ago";
							}
							else {
								cellText = std::to_string(diff) + " days ago";
							}
						}
						else if (ct.tm_hour != lot.tm_hour) {
							int diff = ct.tm_hour - lot.tm_hour;
							if (diff == 1) {
								cellText = "an hour ago";
							}
							else {
								cellText = std::to_string(diff) + " hours ago";
							}
						}
						else if (ct.tm_min != lot.tm_min) {
							int diff = ct.tm_min - lot.tm_min;
							if (diff == 1) {
								cellText = "a minute ago";
							}
							else {
								cellText = std::to_string(diff) + " minutes ago";
							}
						}
						else {
							cellText = "a few seconds ago";
						}
					}
					else {
						cellText = "never";
					}
					break;
				}
				case 3:
					cellText = graphicsApiNames[projectList[row].graphicsApi];
					break;
				}

				if (column == 0) {
					float textWidth = ImGui::CalcTextSize(cellText.c_str(), (const char*)0, false).x;
					float columnWidth = ImGui::GetContentRegionAvail().x - 8;
					int availableCharacters = (int)(columnWidth / (textWidth / cellText.length()));

					if (textWidth > columnWidth) {
						cellText.resize(availableCharacters);
						cellText.resize(availableCharacters + 3, '.');
					}
					
					
					draw_list->ChannelsSplit(2);

					ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0));
					draw_list->ChannelsSetCurrent(1);
					//std::cout << row << std::endl;
					ImGui::Selectable(cellText.c_str(), rowSelected[row], ImGuiSelectableFlags_SpanAllColumns, ImVec2(0, ROW_HEIGHT));
					ImGui::PopStyleVar(1);
					ImGui::PopStyleColor(2);

					if (ImGui::IsItemClicked(1)) {
						selectedProjectId = row;
					}
					
					{
						ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
						ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5);
						ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
						ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(94, 129, 172, 255));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(115, 148, 188, 255));
						ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(76, 86, 106, 255));
						ImGui::SetNextWindowSize(ImVec2(0, 0));
						//std::cout << "Selected Project ID: " << selectedProjectId << " | row: " << row << std::endl;
						if ((selectedProjectId == row) && ImGui::BeginPopupContextItem("Project options")) // <-- use last item id as popup id
						{
							//ImGui::Text("This a popup for \"%s\"!", names[n]);
							if (ImGui::Button("Open in file browser", ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
								std::string projectPath = projectList[row].path;
								size_t pos;
								while ((pos = projectPath.find('\\')) != std::string::npos) {
									projectPath.replace(pos, 1, "/");
								}
								//std::cout << projectPath << std::endl;
								//system(std::string("explorer "" \"" + projectPath + "\" > nul").c_str());

								wchar_t* commandStr = new wchar_t[4096];
								wchar_t* argStr = new wchar_t[4096];
								MultiByteToWideChar(CP_ACP, 0, "explore", -1, commandStr, 4096);
								MultiByteToWideChar(CP_ACP, 0, projectPath.c_str(), -1, argStr, 4096);

								ShellExecute(NULL, commandStr, argStr, NULL, NULL, SW_SHOWNORMAL);
								ImGui::CloseCurrentPopup();
							}

							if (ImGui::IsItemHovered())
								ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

							if (ImGui::Button("Change Graphics API", ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
								state = ProjectChooserState::ChangeGraphicsApi;
								selectedProjectId = row;

								ImGui::CloseCurrentPopup();
							}

							if (ImGui::IsItemHovered())
								ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

							if (ImGui::Button("Remove project from list", ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
								RemoveProjectFromList(row);
								ImGui::CloseCurrentPopup();
							}

							if (ImGui::IsItemHovered())
								ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

							/*if (ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvailWidth(), 0)))
								ImGui::CloseCurrentPopup();

							if (ImGui::IsItemHovered())
								ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);*/
							
							ImGui::EndPopup();
						}
						ImGui::PopStyleVar(4);
						ImGui::PopStyleColor(4);
					}

					if (ImGui::IsItemClicked()) {
						std::cout << "rightclick" << std::endl;
						selectedProjectId = row;
						if (FileHandler::checkIfFileExists((projectList[row].path + "/projectSettings.sproj").c_str())) {
							state = ProjectChooserState::Done;
						}
						else {
							// Error dialog that the project doesn't exist anymore with the option to remove the entry
							state = ProjectChooserState::ProjectNotFound;
						}
					}

					if (ImGui::IsItemHovered()) {
						ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
						draw_list->ChannelsSetCurrent(0);
						ImVec2 p_min = ImGui::GetItemRectMin();
						ImVec2 p_max = ImGui::GetItemRectMax();
						ImU32 rectCol = ImGui::IsMouseDown(ImGuiMouseButton_Left) ? IM_COL32(76, 86, 106, 255) : IM_COL32(67, 76, 94, 255);

						ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, rectCol, 10);
					}

					draw_list->ChannelsMerge();
				}
				else if(column == 1) {
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (ROW_HEIGHT / 2) - ImGui::GetFontSize() / 2);
					float textWidth = ImGui::CalcTextSize(cellText.c_str(), (const char*)0, false).x;
					float columnWidth = ImGui::GetContentRegionAvail().x - 8;
					int availableCharacters = (int)(columnWidth / (textWidth/cellText.length()));
					
					if (textWidth > columnWidth) {
						cellText.resize(availableCharacters);
						cellText.resize(availableCharacters + 3, '.');
					}
					
					ImGui::Text(cellText.c_str());
				}
				else {
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (ROW_HEIGHT / 2) - ImGui::GetFontSize() / 2);
					ImGui::Text(cellText.c_str());
				}
			}
		}
		ImGui::EndTable();
		ImGui::PopFont();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
	}

	ImGui::End();
	//ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(2);
}

void ProjectLauncher::RenderButtons(int fWidth, int fHeight) {
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	//window_flags |= ImGuiWindowFlags_MenuBar;

	ImGui::SetNextWindowPos(ImVec2(fWidth - PROJECT_BUTTON_WINDOW_WIDTH, 0));
	ImGui::SetNextWindowSize(ImVec2(PROJECT_BUTTON_WINDOW_WIDTH, fHeight - PROJECT_INFORMATION_HEIGHT));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(67 / 255.0, 76 / 255.0, 94 / 255.0, 1.0));
	//ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(46 / 255.0, 52 / 255.0, 64 / 255.0, 1.0));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(59 / 255.0, 66 / 255.0, 82 / 255.0, 0));

	if (!ImGui::Begin("Project Buttons", NULL, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::PushFont(headerFont);
	ImGui::Text("Add Project");
	ImGui::PopFont();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6);
	ImGui::Separator();
	ImGui::PushFont(textFont);
	/*if (ImGui::Button("Open project", ImVec2(ImGui::GetContentRegionAvail().x, 75)) && (selectedProjectId != -1)) {
		state = ProjectChooserState::Done;
	}*/
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(76 / 255.0, 86 / 255.0, 106 / 255.0, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(86 / 255.0, 95 / 255.0, 114 / 255.0, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(129 / 255.0, 161 / 255.0, 193 / 255.0, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
	
	if (ImGui::Button("Create new project", ImVec2(ImGui::GetContentRegionAvail().x, 75))) {
		// TODO Open modal for creating a new project
		state = ProjectChooserState::CreateNewProject;
	}

	if (ImGui::IsItemHovered()) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}

	// Put into context menu

	/*if (ImGui::Button("Delete Project from List") && (selectedProjectId != -1)) {
		DeleteProjectFromList(selectedProjectId);
	}*/

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
	
	if (ImGui::Button("Add existing project", ImVec2(ImGui::GetContentRegionAvail().x, 75))) {
		AddProject();
	}

	if (ImGui::IsItemHovered()) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);

	ImGui::PushFont(headerFont);
	ImGui::Text("Options");
	ImGui::PopFont();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6);
	ImGui::Separator();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
	
	if (ImGui::Button("Manage plugins", ImVec2(ImGui::GetContentRegionAvail().x, 75))) {
		// Open plugin manager modal
		// Contains list of plugins that are present and their state (working, not working, warnings)
		// Add plugin button
		// Button to open up the plugin folder
		// Activating and deactivating plugins happens on a project level
	}

	if (ImGui::IsItemHovered()) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}
	
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

	if (ImGui::Button("Settings", ImVec2(ImGui::GetContentRegionAvail().x, 75))) {
		// Open settings modal
		// Manage settings for the launcher and general engine settings?
	}

	if (ImGui::IsItemHovered()) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}

	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(5);
	ImGui::PopFont();
	ImGui::End();
}

void ProjectLauncher::RenderBottomInformation(int fWidth, int fHeight) {
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	//window_flags |= ImGuiWindowFlags_MenuBar;

	ImGui::SetNextWindowPos(ImVec2(0, fHeight - PROJECT_INFORMATION_HEIGHT));
	ImGui::SetNextWindowSize(ImVec2(fWidth, PROJECT_INFORMATION_HEIGHT));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(59 / 255.0, 66 / 255.0, 82 / 255.0, 1.0));
	//ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(46 / 255.0, 52 / 255.0, 64 / 255.0, 1.0));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(59 / 255.0, 66 / 255.0, 82 / 255.0, 0));
	ImGui::PushFont(textFont);

	if (!ImGui::Begin("Information", NULL, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::Text("An open source project created by flash-miller");
	const char* versionText = "early development version";
	float textWidth = ImGui::CalcTextSize(versionText, (const char*)0, false).x;


	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(229, 233, 240, 255));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 0));
	static ImVec2 iconSize = ImVec2(18,18);
	static float verticalIconOffset = 2;
	
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - verticalIconOffset);
		ImGui::Image((ImTextureID)githubIcon->ID, iconSize);
		if (ImGui::IsItemHovered()) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}
		if (ImGui::IsItemClicked()) {
			ShellExecute(0, 0, L"https://github.com/flash-miller/simple-3d-engine", 0, 0, SW_SHOW);
		}
		
		ImGui::SameLine();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + verticalIconOffset);
		ImGui::Text("GitHub");
		if (ImGui::IsItemHovered()) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}
		if (ImGui::IsItemClicked()) {
			ShellExecute(0, 0, L"https://github.com/flash-miller/simple-3d-engine", 0, 0, SW_SHOW);
		}
	}

	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
	
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - verticalIconOffset);
		ImGui::Image((ImTextureID)patreonIcon->ID, iconSize);
		if (ImGui::IsItemHovered()) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}
		if (ImGui::IsItemClicked()) {
			ShellExecute(0, 0, L"https://patreon.com/flash-miller", 0, 0, SW_SHOW);
		}
		
		ImGui::SameLine();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + verticalIconOffset);
		ImGui::Text("Patreon");
		if (ImGui::IsItemHovered()) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}
		if (ImGui::IsItemClicked()) {
			ShellExecute(0, 0, L"https://patreon.com/flash-miller", 0, 0, SW_SHOW);
		}
	}

	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - verticalIconOffset);
		ImGui::Image((ImTextureID)twitterIcon->ID, iconSize);
		if (ImGui::IsItemHovered()) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}
		if (ImGui::IsItemClicked()) {
			ShellExecute(0, 0, L"https://twitter.com/flash_miller", 0, 0, SW_SHOW);
		}
		
		ImGui::SameLine();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + verticalIconOffset);
		ImGui::Text("Twitter");
		if (ImGui::IsItemHovered()) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}
		if (ImGui::IsItemClicked()) {
			ShellExecute(0, 0, L"https://twitter.com/flash_miller", 0, 0, SW_SHOW);
		}
	}
	
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() - textWidth);
	ImGui::Text(versionText);

	ImGui::End();
	ImGui::PopStyleColor(2);
	ImGui::PopFont();
}

void ProjectLauncher::RenderChangeGraphicsApiModal() {
	// Centering Modal
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	// Setting the dimming background color
	ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0, 0, 0, 0.7));

	// Setting the font for the modal window title
	ImGui::PushFont(headerFont);
	ImGui::SetNextWindowSize(ImVec2(425, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(46, 52, 64, 255));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(94, 129, 172, 255));

	// ==================== Popup Modal ====================
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));

	// ==================== Popup Modal ====================
	if (ImGui::BeginPopupModal("Change Graphics API", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
		ImGui::PushFont(textFont);
		// ==================== Information Text ====================
		ImGui::Text("You are about to change the graphics API of a project.\nThis can break the project.\n\
			\n\nIf you project becomes unable to launch, you can safely revert back\nto the previous graphics API.\
			\n\nPlease make sure that:\
			\n- Custom shaders have an equivelant to the selected graphics API\
			\n- Used plugins are compatible with the graphics API you choose\
			\n- Added source code supports the graphics API");

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

		ImGui::Separator();

		/*ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(191, 97, 106, 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(198, 111, 120, 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(216, 134, 142, 255));*/
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

		static int currentApiIndex = (int)projectList[selectedProjectId].graphicsApi;
		// ==================== Graphics API Selection Dropdown ====================
		{
			// Graphics Title
			ImGui::PushFont(headerFont);
			ImGui::Text("Graphics API");
			ImGui::PopFont();

			//ImGui::Separator();
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() - 150);

			// Setting up combo
			const char* apis[] = { "OpenGL", "Direct3D 11", "Direct3D 12", "Vulkan" };
			const char* combo_preview_value = apis[currentApiIndex];

			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
			ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));
			ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
			ImGui::PushFont(textFont);

			// Coloring the combo preview
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(59, 66, 82, 255));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(76, 86, 106, 255));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(255, 0, 0, 255));

			// Coloring the selected combo item
			ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(59, 66, 82, 255));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(67, 76, 94, 255));

			// Coloring the combo popup background
			ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(46, 52, 64, 255));

			if (ImGui::BeginCombo("##graphicsApi", combo_preview_value)) {
				ImDrawList* draw_list = ImGui::GetWindowDrawList();

				// Looping through all the combo entries
				for (int n = 0; n < 4; n++)
				{
					const bool is_selected = (currentApiIndex == n);

					// Selectable settings
					ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));

					// For drawing a custom Selectable background, we split the channel
					// Now we can draw the text in the foreground, and the colored, rounded rectangle in the background
					draw_list->ChannelsSplit(2);
					draw_list->ChannelsSetCurrent(1);

					if (n > 0) {
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
					}

					// ==================== Graphics Selectable ====================
					if (ImGui::Selectable(apis[n], is_selected, 0, ImVec2(0, 16 + 10))) {
						currentApiIndex = n;
					}

					// Reset Style
					ImGui::PopStyleVar(1);
					ImGui::PopStyleColor(3);

					// Drawing the background rectangle
					if (ImGui::IsItemHovered()) {
						ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
						draw_list->ChannelsSetCurrent(0);
						ImVec2 p_min = ImGui::GetItemRectMin();
						ImVec2 p_max = ImGui::GetItemRectMax();
						ImU32 rectCol = ImGui::IsMouseDown(ImGuiMouseButton_Left) ? IM_COL32(76, 86, 106, 255) : IM_COL32(67, 76, 94, 255);

						ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, rectCol, 5);
					}
					else if (is_selected) {
						draw_list->ChannelsSetCurrent(0);
						ImVec2 p_min = ImGui::GetItemRectMin();
						ImVec2 p_max = ImGui::GetItemRectMax();
						ImU32 rectCol = IM_COL32(59, 66, 82, 255);

						ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, rectCol, 5);
					}

					draw_list->ChannelsMerge();

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
			ImGui::PopFont();

			// Use Hand cursor when hovering over the combo
			if (ImGui::IsItemHovered()) {
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}

			// Reset remaining style attributes
			ImGui::PopStyleVar(4);
			ImGui::PopStyleColor(6);
		}

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
		ImGui::Separator();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

		ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(76, 86, 106, 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(86, 95, 114, 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(129, 161, 193, 255));
		
		// ==================== Apply Button ====================
		if (ImGui::Button("Apply", ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - 10, 0))) {
			ChangeGraphicsApi(selectedProjectId, (GraphicsApi)currentApiIndex);
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::IsItemHovered()) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}

		//ImGui::PopStyleColor(3);

		// ==================== Cancel Button ====================
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::IsItemHovered()) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}


		ImGui::PopStyleVar();
		ImGui::PopStyleColor(3);

		ImGui::PopFont();

		ImGui::EndPopup();
	}
	ImGui::PopStyleVar(5);
	ImGui::PopFont();
	ImGui::PopStyleColor(3);
}

void ProjectLauncher::ChangeGraphicsApi(int id, GraphicsApi api) {
	//ProjectInformation info = ExtractProjectInformation(projectList[id].path.c_str());
	std::string projectSettingsPath = (projectList[id].path + "/projectSettings.sproj").c_str();
	size_t pos;
	while ((pos = projectSettingsPath.find('\\')) != std::string::npos) {
		projectSettingsPath.replace(pos, 1, "/");
	}
	
	if (FileHandler::checkIfFileExists(projectSettingsPath.c_str())) {
		// Update the projectSettings.sproj "projectName" entry
		//std::string templateProjectSettingsDirectory = templateProjectDirectory + "/projectSettings.sproj";
		std::string fileContent = FileHandler::readFile(projectSettingsPath.c_str());

		if (!fileContent.empty()) {
			nlohmann::json jsonObject = nlohmann::json::parse(fileContent);
			if (jsonObject["projectInformation"]["graphicsApi"] != (int)api) {
				jsonObject["projectInformation"]["graphicsApi"] = (int)api;

				std::string newFileContent = jsonObject.dump(4);
				FileHandler::writeToFile(projectSettingsPath.c_str(), newFileContent);
				projectList[selectedProjectId].graphicsApi = (int)api;
				SaveProjectList();
			}
		}
		else {
			std::cout << "yikes bro" << std::endl;
		}
	}
	else {
		// wtf
		std::cout << "There is no projectSettings.json file to be found for the mentioned project!" << std::endl;
	}
}

void ProjectLauncher::RenderProjectNotFoundModal() {
	// Centering Modal
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	// Setting the dimming background color
	ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0, 0, 0, 0.7));

	// Setting the font for the modal window title
	ImGui::PushFont(headerFont);
	ImGui::SetNextWindowSize(ImVec2(350, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(46, 52, 64, 255));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(94, 129, 172, 255));

	// ==================== Popup Modal ====================
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));

	// ==================== Popup Modal ====================
	if (ImGui::BeginPopupModal("Project not found!", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
		ImGui::PushFont(textFont);
		// ==================== Information Text ====================
		std::string pathText = projectList[selectedProjectId].path;

		float textWidth = ImGui::CalcTextSize(pathText.c_str(), (const char*)0, false).x;
		
		ImGui::Text("The project you are trying to open could not be found:");

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvailWidth() / 2 - textWidth / 2));
		ImGui::Text(projectList[selectedProjectId].path.c_str());
		
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
		ImGui::Text("Do you want to remove it from the list?");

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
		
		ImGui::Separator();

		ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(191, 97, 106, 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(198, 111, 120, 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(216, 134, 142, 255));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

		// ==================== Remove Button ====================
		if (ImGui::Button("Remove", ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - 10, 0))) {
			RemoveProjectFromList(selectedProjectId);
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::IsItemHovered()) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}

		ImGui::PopStyleColor(3);
		ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(76, 86, 106, 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(86, 95, 114, 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(129, 161, 193, 255));

		// ==================== Cancel Button ====================
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
			ImGui::CloseCurrentPopup();
		}
		
		if (ImGui::IsItemHovered()) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}


		ImGui::PopStyleVar();
		ImGui::PopStyleColor(3);
		
		ImGui::PopFont();
		
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar(5);
	ImGui::PopFont();
	ImGui::PopStyleColor(3);
}

void ProjectLauncher::RenderCreateNewProjectModal() {
	// Centering Modal ====================
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	
	// Setting the dimming background color
	ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0, 0, 0 , 0.7));
	
	// Setting the font for the modal window title
	ImGui::PushFont(headerFont);
	ImGui::SetNextWindowSize(ImVec2(450, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(46, 52, 64, 255));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(94, 129, 172, 255));
	
	// ==================== Popup Modal ====================
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
	if (ImGui::BeginPopupModal("Create new project", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize )) {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 8));

		static char newProjectName[128] = "";
		// ==================== Name Input ====================
		{
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			// Title
			ImGui::PushFont(headerFont);
			ImGui::Text("Name");
			ImGui::PopFont();

			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() - 200);

			// Text Input
			ImGui::PushFont(textFont);
			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(59, 66, 82, 255));
			ImGui::InputTextWithHint("##projectName", "Enter name", newProjectName, IM_ARRAYSIZE(newProjectName));
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor();
			ImGui::PopItemWidth();
			ImGui::PopFont();
		}

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
		ImGui::Separator();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
		
		static std::string newProjectPath = defaultProjectPath;
		// ==================== Project Path Selector ====================
		{
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			// Title
			ImGui::PushFont(headerFont);
			ImGui::Text("Path");
			ImGui::PopFont();

			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() - 300);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			
			draw_list->ChannelsSplit(2);
			draw_list->ChannelsSetCurrent(1);
			// Path preview
			ImGui::PushFont(textFont);
			std::string renderText = newProjectPath;

			float pathPreviewWidth = ImGui::GetContentRegionAvailWidth() - 35;

			draw_list->ChannelsSetCurrent(1);
			float textWidth = ImGui::CalcTextSize(renderText.c_str(), (const char*)0, false).x;
			float columnWidth = ImGui::GetContentRegionAvail().x - 40;
			int availableCharacters = (int)(columnWidth / (textWidth / renderText.length()))-3;

			if ((textWidth > columnWidth)) {
				renderText.resize(availableCharacters);
				renderText.resize(availableCharacters + 3, '.');
			}
			ImGui::Text(renderText.c_str());

			draw_list->ChannelsSetCurrent(0);
			ImVec2 p_min = ImVec2(ImGui::GetItemRectMin().x - 5, ImGui::GetItemRectMin().y - 5);
			ImVec2 p_max = ImVec2(ImGui::GetItemRectMin().x + pathPreviewWidth, ImGui::GetItemRectMax().y + 5);

			ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, IM_COL32(59, 66, 82, 255), 3);

			draw_list->ChannelsMerge();
			
			ImGui::SameLine();

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() - 30);

			float width = ImGui::GetContentRegionAvailWidth();
			
			// Browse path button
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			if (ImGui::Button("...", ImVec2(width,25))) {
				// Directory Dialog
				// File Dialog um einen Pfad zu kriegen
				// Im Pfad nach einer projectSettings.sproj suchen
				// Projectinformation (Name, Pfad, letzte Modifikationszeit)
				nfdchar_t* outPath = NULL;
				const nfdchar_t* filter = "sproj";

				// TODO implement file dialog in another thread to not interrupt
				// TODO rename "open" button to "add project" or something
				// TODO focus on the opened dialog when trying to return to the project launcher window 
				//(if that is not the default behaviour, finding out when implementing the dialog in another thread)
				//nfdresult_t result = NFD_OpenDialog(filter, NULL, &outPath);
				nfdresult_t result = NFD_PickFolder((const nfdchar_t*)newProjectPath.c_str(), &outPath);

				if (result == NFD_OKAY) {
					newProjectPath = std::string(outPath);
					free(outPath);
				}
				else if (result == NFD_CANCEL) {
					//puts("User pressed cancel.");
				}
				else {
					//printf("Error: %s\n", NFD_GetError());
				}
			}
			ImGui::PopStyleVar(2);

			if (ImGui::IsItemHovered()) {
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}


			ImGui::PopFont();
		}

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
		ImGui::Separator();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

		static int currentTemplateIndex = 0;
		// ==================== Project Template Selector ====================
		{
			// Template Title
			ImGui::PushFont(headerFont);
			ImGui::Text("Template");
			ImGui::PopFont();

			//ImGui::Separator();
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() - 150);

			// Setting up combo
			const char* templates[] = { "Blank", "Demo1" };
			const char* template_preview_value = templates[currentTemplateIndex];

			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
			ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));
			//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
			ImGui::PushFont(textFont);

			// Coloring the combo preview
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(59, 66, 82, 255));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(76, 86, 106, 255));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(255, 0, 0, 255));

			// Coloring the selected combo item
			ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(59, 66, 82, 255));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(67, 76, 94, 255));

			// Coloring the combo popup background
			ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(46, 52, 64, 255));

			if (ImGui::BeginCombo("##template", template_preview_value)) {
				ImDrawList* draw_list = ImGui::GetWindowDrawList();

				// Looping through all the combo entries
				for (int n = 0; n < 2; n++)
				{
					const bool is_selected = (currentTemplateIndex == n);

					// Selectable settings
					ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));

					// For drawing a custom Selectable background, we split the channel
					// Now we can draw the text in the foreground, and the colored, rounded rectangle in the background
					draw_list->ChannelsSplit(2);
					draw_list->ChannelsSetCurrent(1);
					if (n > 0) {
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
					}

					//ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(8, 8));
					// ==================== Rendering Selectable ====================
					if (ImGui::Selectable(templates[n], is_selected, 0, ImVec2(0, 16 + 10))) {
						currentTemplateIndex = n;
					}
					//ImGui::PopStyleVar();

					// Reset Style
					ImGui::PopStyleVar(1);
					ImGui::PopStyleColor(3);

					// Drawing the background rectangle
					if (ImGui::IsItemHovered()) {
						ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
						draw_list->ChannelsSetCurrent(0);
						ImVec2 p_min = ImGui::GetItemRectMin();
						ImVec2 p_max = ImGui::GetItemRectMax();
						ImU32 rectCol = ImGui::IsMouseDown(ImGuiMouseButton_Left) ? IM_COL32(76, 86, 106, 255) : IM_COL32(67, 76, 94, 255);

						ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, rectCol, 5);
					}
					else if (is_selected) {
						draw_list->ChannelsSetCurrent(0);
						ImVec2 p_min = ImGui::GetItemRectMin();
						ImVec2 p_max = ImGui::GetItemRectMax();
						ImU32 rectCol = IM_COL32(59, 66, 82, 255);

						ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, rectCol, 5);
					}

					draw_list->ChannelsMerge();

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
			ImGui::PopFont();

			// Use Hand cursor when hovering over the combo
			if (ImGui::IsItemHovered()) {
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}

			// Reset remaining style attributes
			ImGui::PopStyleVar(4);
			ImGui::PopStyleColor(6);
		}

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
		ImGui::Separator();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

		static int currentApiIndex = 0;
		// ==================== Graphics API Selection Dropdown ====================
		{
			// Graphics Title
			ImGui::PushFont(headerFont);
			ImGui::Text("Graphics API");
			ImGui::PopFont();
			
			//ImGui::Separator();
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() - 150);

			// Setting up combo
			const char* apis[] = { "OpenGL", "Direct3D 11", "Direct3D 12", "Vulkan" };
			const char* combo_preview_value = apis[currentApiIndex];
			
			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
			ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));
			ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
			ImGui::PushFont(textFont);

			// Coloring the combo preview
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(59, 66, 82, 255));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(76, 86, 106, 255));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(255, 0, 0, 255));

			// Coloring the selected combo item
			ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(59, 66, 82, 255));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(67, 76, 94, 255));

			// Coloring the combo popup background
			ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(46, 52, 64, 255));

			if (ImGui::BeginCombo("##graphicsApi", combo_preview_value)) {
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				
				// Looping through all the combo entries
				for (int n = 0; n < 4; n++)
				{
					const bool is_selected = (currentApiIndex == n);

					// Selectable settings
					ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));
					
					// For drawing a custom Selectable background, we split the channel
					// Now we can draw the text in the foreground, and the colored, rounded rectangle in the background
					draw_list->ChannelsSplit(2);
					draw_list->ChannelsSetCurrent(1);

					if (n > 0) {
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
					}

					// ==================== Rendering Selectable ====================
					if (ImGui::Selectable(apis[n], is_selected, 0, ImVec2(0, 16 + 10))) {
						currentApiIndex = n;
					}

					// Reset Style
					ImGui::PopStyleVar(1);
					ImGui::PopStyleColor(3);
					
					// Drawing the background rectangle
					if (ImGui::IsItemHovered()) {
						ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
						draw_list->ChannelsSetCurrent(0);
						ImVec2 p_min = ImGui::GetItemRectMin();
						ImVec2 p_max = ImGui::GetItemRectMax();
						ImU32 rectCol = ImGui::IsMouseDown(ImGuiMouseButton_Left) ? IM_COL32(76, 86, 106, 255) : IM_COL32(67, 76, 94, 255);

						ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, rectCol, 5);
					}
					else if (is_selected) {
						draw_list->ChannelsSetCurrent(0);
						ImVec2 p_min = ImGui::GetItemRectMin();
						ImVec2 p_max = ImGui::GetItemRectMax();
						ImU32 rectCol = IM_COL32(59, 66, 82, 255);

						ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, rectCol, 5);
					}

					draw_list->ChannelsMerge();

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
			ImGui::PopFont();

			// Use Hand cursor when hovering over the combo
			if (ImGui::IsItemHovered()) {
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}

			// Reset remaining style attributes
			ImGui::PopStyleVar(4);
			ImGui::PopStyleColor(6);
		}

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
		ImGui::Separator();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(76 / 255.0, 86 / 255.0, 106 / 255.0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(86 / 255.0, 95 / 255.0, 114 / 255.0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(129 / 255.0, 161 / 255.0, 193 / 255.0, 1));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);

		// ==================== Create Button ====================
		{
			ImGui::PushFont(textFont);
			if (ImGui::Button("Create", ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - 8, 40))) {
				// Create folder at the path with the project name
				// Create and fill projectSettings.sproj appropriately
				// If using a template, copy the corresponding files
				// Add project to saved project list
				// Launch project after creating, or just close the popup?
				CreateProject(newProjectName, newProjectPath.c_str(), (GraphicsApi)currentApiIndex, (ProjectTemplate)currentTemplateIndex);
				
				strcpy_s(newProjectName, "");
				newProjectPath = defaultProjectPath;
				currentTemplateIndex = 0;
				currentApiIndex = 0;
				
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopFont();

			if (ImGui::IsItemHovered()) {
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}
		}

		// ==================== Cancel Button ====================
		{
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			ImGui::PushFont(textFont);
			if (ImGui::Button("Cancel", ImVec2(ImGui::GetContentRegionAvailWidth(), 40))) {
				strcpy_s(newProjectName, "");
				newProjectPath = defaultProjectPath;
				currentTemplateIndex = 0;
				currentApiIndex = 0;
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopFont();

			if (ImGui::IsItemHovered()) {
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}
		}

		ImGui::PopStyleVar();
		ImGui::PopStyleColor(3);
		
		ImGui::PopStyleVar();
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar(5);
	ImGui::PopFont();
	ImGui::PopStyleColor(3);
}

void ProjectLauncher::AddProjectWrapper() {
	std::thread thread_object(&ProjectLauncher::AddProject, this);
	thread_object.detach();
}


void ProjectLauncher::AddProject() {
	// File Dialog um einen Pfad zu kriegen
	// Im Pfad nach einer projectSettings.sproj suchen
	// Projectinformation (Name, Pfad, letzte Modifikationszeit)
	nfdchar_t* outPath = NULL;
	const nfdchar_t* filter = "sproj";

	// TODO implement file dialog in another thread to not interrupt
	// TODO rename "open" button to "add project" or something
	// TODO focus on the opened dialog when trying to return to the project launcher window
	//(if that is not the default behaviour, finding out when implementing the dialog in another thread)
	//nfdresult_t result = NFD_OpenDialog(filter, NULL, &outPath);
	nfdresult_t result = NFD_PickFolder(NULL, &outPath);
	
	if (result == NFD_OKAY) {
		// check if a project with the same path already exists
		bool alreadyPresent = false;
		for (int i = 0; (i < projectList.size()) && !alreadyPresent; i++) {
			alreadyPresent = projectList[i].path == outPath;
		}
		
		if (!alreadyPresent) {
			ProjectInformation newProject = ExtractProjectInformation(outPath);
			if (newProject.name != "") {

				projectList.push_back(newProject);
				SortProjectList();
				// Aktualisierte Projektliste speichern
				SaveProjectList();
			}
			else {
				// TODO: Open modal to signal that no project could be found at the given path
			}
		}
		else {
			// TODO: Modal to notify the project is already present in the current project list
		}
		free(outPath);
	}
	else if (result == NFD_CANCEL) {
		//puts("User pressed cancel.");
	}
	else {
		//printf("Error: %s\n", NFD_GetError());
	}
}

void ProjectLauncher::AddProjectToList(ProjectInformation projectInformation) {
	bool alreadyPresent = false;
	for (int i = 0; (i < projectList.size()) && !alreadyPresent; i++) {
		alreadyPresent = projectList[i].path == projectInformation.path;
	}

	if (!alreadyPresent) {
		projectList.push_back(projectInformation);
		SortProjectList();
		// Aktualisierte Projektliste speichern
		SaveProjectList();
	}
}

void ProjectLauncher::LoadProjectList() {
	std::string savedProjectsPath = GetSettingsPath() + "savedProjects.json";
	std::string fileContent = FileHandler::readFile(savedProjectsPath.c_str());
	if (!fileContent.empty()) {
		nlohmann::json jsonObject = nlohmann::json::parse(fileContent);

		if (jsonObject.contains("projects")) {
			for (int i = 0; i < jsonObject["projects"].size(); i++) {
				ProjectInformation projectToAdd;
				if (jsonObject["projects"][i].contains("name")) {
					projectToAdd.name = jsonObject["projects"][i]["name"];
				}

				if (jsonObject["projects"][i].contains("path")) {
					projectToAdd.path = jsonObject["projects"][i]["path"];
				}

				if (jsonObject["projects"][i].contains("lastOpened")) {
					projectToAdd.lastOpened = jsonObject["projects"][i]["lastOpened"];
				}

				if (jsonObject["projects"][i].contains("graphicsApi")) {
					projectToAdd.graphicsApi = jsonObject["projects"][i]["graphicsApi"];
				}

				projectList.push_back({
						projectToAdd
					});
			}

			// After adding all the projects, sort them
			SortProjectList();
		}
	}
}

void ProjectLauncher::RemoveProjectFromList(int id) {
	projectList.erase(projectList.begin() + id);
	SaveProjectList();
}

void ProjectLauncher::SaveProjectList() {
	nlohmann::json jsonObject;
	for (int i = 0; i < projectList.size(); i++) {
		jsonObject["projects"][i]["name"] = projectList[i].name;
		jsonObject["projects"][i]["path"] = projectList[i].path;
		jsonObject["projects"][i]["lastOpened"] = projectList[i].lastOpened;
		jsonObject["projects"][i]["graphicsApi"] = projectList[i].graphicsApi;
	}

	std::string fileContent = jsonObject.dump(4);
	std::string settingsPath = GetSettingsPath();
	if (!FileHandler::checkIfDirectoyExists(settingsPath)) {
		FileHandler::createDirectoryS(GetSettingsPath());
	}
	std::string savedProjectsPath = settingsPath + "savedProjects.json";
	//std::cout << "Saving savedProjects.json to: " << GetSettingsPath() << std::endl;
	FileHandler::writeToFile(savedProjectsPath.c_str(), fileContent);
}

ProjectInformation ProjectLauncher::ExtractProjectInformation(const char* path) {
	std::string projectSettingsPath = (std::string(path) + "/projectSettings.sproj");
	std::string fileContent = FileHandler::readFile(projectSettingsPath.c_str());
	
	ProjectInformation foundInfo;

	if (!fileContent.empty()) {
		nlohmann::json jsonObject = nlohmann::json::parse(fileContent);

		if (jsonObject.contains("projectInformation")) {
			foundInfo.name = jsonObject["projectInformation"]["projectName"];
			foundInfo.path = path;
			foundInfo.graphicsApi = jsonObject["projectInformation"]["graphicsApi"];
			foundInfo.lastOpened = -1;
		}
	}

	return foundInfo;
}

int ProjectLauncher::CreateProject(const char* projectName, const char* projectPath, GraphicsApi graphicsApi, ProjectTemplate projectTemplate) {
	std::string projectDirectory = std::string(projectPath) + "/" + projectName;
	if (!FileHandler::checkIfDirectoyExists(projectDirectory)) {
		std::string projectSettingsPath = (std::string(projectPath) + "/" + projectName + "/projectSettings.sproj").c_str();
		size_t pos;
		while ((pos = projectSettingsPath.find('\\')) != std::string::npos) {
			projectSettingsPath.replace(pos, 1, "/");
		}
		if (!FileHandler::checkIfFileExists(projectSettingsPath.c_str())) {
			FileHandler::createDirectoryS(projectDirectory);

			if (projectTemplate == ProjectTemplate::Blank) {
				nlohmann::json jsonObject;
				jsonObject["projectInformation"]["projectName"] = projectName;
				jsonObject["projectInformation"]["graphicsApi"] = (int)graphicsApi;

				std::string fileContent = jsonObject.dump(4);
				FileHandler::writeToFile((std::string(projectPath) + "/" + projectName + "/projectSettings.sproj").c_str(), fileContent);
			}
			else {
				std::string templateProjectDirectory = "";
				std::string templateApiDirectory = "";

				switch (projectTemplate) {
				case ProjectTemplate::Demo1:
					templateProjectDirectory = "demo1";
					break;
				}

				switch (graphicsApi) {
				case GraphicsApi::OpenGL:
					templateApiDirectory = "opengl";
					break;
				case GraphicsApi::D3D11:
					templateApiDirectory = "dx11";
					break;
				case GraphicsApi::D3D12:
					templateApiDirectory = "dx12";
					break;
				case GraphicsApi::Vulkan:
					templateApiDirectory = "vulkan";
					break;
				}

				templateProjectDirectory = "data/demo projects/" + templateApiDirectory + "/" + templateProjectDirectory;

				std::string copyCommand = std::string("Xcopy \"" + templateProjectDirectory
					+ "\" \""
					+ std::string(projectPath) + "/" + projectName
					+ "\" /E/H/C/I/y/D");
				
				//std::cout << copyCommand << std::endl;

				// Update the projectSettings.sproj "projectName" entry
				std::string templateProjectSettingsDirectory = templateProjectDirectory + "/projectSettings.sproj";
				std::string fileContent = FileHandler::readFile(templateProjectSettingsDirectory.c_str());

				if (!fileContent.empty()) {
					nlohmann::json jsonObject = nlohmann::json::parse(fileContent);
					jsonObject["projectInformation"]["projectName"] = projectName;

					std::string newFileContent = jsonObject.dump(4);
					FileHandler::writeToFile(projectSettingsPath.c_str(), newFileContent);
				}
				else {
					std::cout << "yikes bro" << std::endl;
				}


				system(copyCommand.c_str());
			}

			size_t pos;
			while ((pos = projectDirectory.find('/')) != std::string::npos) {
				projectDirectory.replace(pos, 1, "\\");
			}

			ProjectInformation newProjectInformation;
			newProjectInformation.name = projectName;
			newProjectInformation.path = projectDirectory;
			newProjectInformation.lastOpened = -1;
			newProjectInformation.graphicsApi = (int)graphicsApi;

			AddProjectToList(newProjectInformation);

			return 0;
		}
		else {
			// wtf
			std::cout << "projectSettings.json already exists at project directory!" << std::endl;
			return 1;
		}
	}
	else {
		// Project folder already exists
		std::cout << "Project folder already exists at: " << (std::string(projectPath) + "/" + projectName) << std::endl;
		return 2;
	}
}

void ProjectLauncher::UpdateSortOrder(int columnId) {
	switch (columnId) {
	case 0:
		if (sortOrder == ProjectSortOrder::Name) {
			sortOrder = ProjectSortOrder::NameReverse;
		}
		else {
			sortOrder = ProjectSortOrder::Name;
		}
		SortProjectList();
		break;
	case 1: break;
	case 2:
		if (sortOrder == ProjectSortOrder::Date) {
			sortOrder = ProjectSortOrder::DateReverse;
		}
		else {
			sortOrder = ProjectSortOrder::Date;
		}
		SortProjectList();
		break;
	case 3:
		if (sortOrder == ProjectSortOrder::Api) {
			sortOrder = ProjectSortOrder::ApiReverse;
		}
		else {
			sortOrder = ProjectSortOrder::Api;
		}
		SortProjectList();
		break;
	}
}

bool projectNameComparator(ProjectInformation p1, ProjectInformation p2) {
	return p1.name > p2.name;
}

bool projectNameReverseComparator(ProjectInformation p1, ProjectInformation p2) {
	return p1.name < p2.name;
}

bool projectDateComparator(ProjectInformation p1, ProjectInformation p2) {
	return p1.lastOpened > p2.lastOpened;
}

bool projectDateReverseComparator(ProjectInformation p1, ProjectInformation p2) {
	return p1.lastOpened < p2.lastOpened;
}

bool projectApiComparator(ProjectInformation p1, ProjectInformation p2) {
	return p1.graphicsApi > p2.graphicsApi;
}

bool projectApiReverseComparator(ProjectInformation p1, ProjectInformation p2) {
	return p1.graphicsApi < p2.graphicsApi;
}

void ProjectLauncher::SortProjectList() {
	switch (sortOrder) {
	case ProjectSortOrder::Name:
		std::sort(projectList.begin(), projectList.end(), projectNameComparator);
		break;
	case ProjectSortOrder::NameReverse:
		std::sort(projectList.begin(), projectList.end(), projectNameReverseComparator);
		break;
	case ProjectSortOrder::Date:
		std::sort(projectList.begin(), projectList.end(), projectDateComparator);
		break;
	case ProjectSortOrder::DateReverse:
		std::sort(projectList.begin(), projectList.end(), projectDateReverseComparator);
		break;
	case ProjectSortOrder::Api:
		std::sort(projectList.begin(), projectList.end(), projectApiComparator);
		break;
	case ProjectSortOrder::ApiReverse:
		std::sort(projectList.begin(), projectList.end(), projectApiReverseComparator);
		break;
	}
}