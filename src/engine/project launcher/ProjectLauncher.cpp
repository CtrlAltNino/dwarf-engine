#include "ProjectLauncher.h"
#define INITIAL_WINDOW_WIDTH (960)
#define INITIAL_WINDOW_HEIGHT (540)
#define PROJECT_BUTTON_WINDOW_WIDTH (200)
#define PROJECT_INFORMATION_HEIGHT (30)

ProjectLauncher::ProjectLauncher() {
	
}

ProjectReturnData ProjectLauncher::Run() {
	glfwInit();
	LoadProjectList();
	if (window == nullptr) {
		CreateWindow();
	}

	InitializeIMGUI();

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
		projectData.renderingApi = selectedApi;
		projectList[selectedProjectId].lastOpened = time(0);
		SaveProjectList();
	}
	
	return projectData;
}

int ProjectLauncher::CreateWindow() {
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

	window = glfwCreateWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "Simple 3D Engine - Project Launcher", NULL, NULL);
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

	RenderProjectNotFoundModal();
	RenderCreateNewProjectModal();

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
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(59 / 255.0, 66 / 255.0, 82 / 255.0, 0));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(46 / 255.0, 52 / 255.0, 64 / 255.0, 1.0));

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
	ImGui::PushStyleColor(ImGuiCol_TableBorderLight, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, IM_COL32(59, 66, 82, 255));
	// /*ImGuiTableFlags_Borders | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable |*/ ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_RowBg
	if (ImGui::BeginTable("Project entries", COLUMNS_COUNT, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp))
	{
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Path");
		ImGui::TableSetupColumn("Last opened");
		ImGui::TableSetupColumn("API");
		//ImGui::Columns(3);
		//ImGui::SetColumnWidth(0, 50);

		// Dummy entire-column selection storage
		// FIXME: It would be nice to actually demonstrate full-featured selection using those checkbox.
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		std::fill(rowSelected, rowSelected + projectList.size(), false);

		// Instead of calling TableHeadersRow() we'll submit custom headers ourselves
		ImGui::TableNextRow(ImGuiTableRowFlags_Headers, HEADER_ROW_HEIGHT);
		ImGui::PushFont(headerFont);
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(76, 86, 106, 255));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, IM_COL32(129, 161, 193, 255));
		ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 10);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 10);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10);
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
		ImGui::PopStyleVar(3);
		ImGui::PopFont();

		//ImGui::Separator();
		ImGui::PushFont(textFont);
		for (int row = 0; row < projectList.size(); row++)
		{
			ImGui::TableNextRow(ImGuiTableRowFlags_None, ROW_HEIGHT);
			for (int column = 0; column < COLUMNS_COUNT; column++)
			{
				ImGui::TableSetColumnIndex(column);
				std::string cellText = "";
				switch (column) {
				case 0:
					ImGui::PushItemWidth(40);
					cellText = projectList[row].name.c_str();
					break;
				case 1:
					ImGui::PushItemWidth(100);
					cellText = projectList[row].path.c_str();
					break;
				case 2:
				{
					ImGui::PushItemWidth(35);
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
					ImGui::PushItemWidth(45);
					cellText = projectList[row].renderingApi.c_str();
					break;
				}

				if (column == 0) {
					// TODO: Implement right click menu for projects (open project in explorer, delete project, remove from list)
					//ImGui::GetWindowContentRegionWidth();
					draw_list->ChannelsSplit(2);

					ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0));
					draw_list->ChannelsSetCurrent(1);
					//std::cout << row << std::endl;
					ImGui::Selectable(cellText.c_str(), rowSelected[row], ImGuiSelectableFlags_SpanAllColumns, ImVec2(0, ROW_HEIGHT));
					ImGui::PopStyleVar(1);
					ImGui::PopStyleColor(2);

					if (ImGui::IsItemClicked()) {
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
					
					if ((column == 1) && (textWidth > columnWidth)) {
						cellText.resize(availableCharacters);
						cellText.resize(availableCharacters + 3, '.');
					}
					
					ImGui::Text(cellText.c_str());
				}
				else {
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (ROW_HEIGHT / 2) - ImGui::GetFontSize() / 2);
					ImGui::Text(cellText.c_str());
				}
				
				if(column != 1)
					ImGui::PopItemWidth();
			}
		}
		ImGui::EndTable();
		ImGui::PopFont();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
	}

	ImGui::End();
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

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 10));
	ImGui::PushFont(headerFont);
	ImGui::Text("Add Project");
	ImGui::PopFont();
	ImGui::Separator();
	ImGui::PushFont(textFont);
	/*if (ImGui::Button("Open project", ImVec2(ImGui::GetContentRegionAvail().x, 75)) && (selectedProjectId != -1)) {
		state = ProjectChooserState::Done;
	}*/
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(76 / 255.0, 86 / 255.0, 106 / 255.0, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(86 / 255.0, 95 / 255.0, 114 / 255.0, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(129 / 255.0, 161 / 255.0, 193 / 255.0, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);
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

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 20));
	if (ImGui::Button("Add existing project", ImVec2(ImGui::GetContentRegionAvail().x, 75))) {
		AddProject();
	}

	if (ImGui::IsItemHovered()) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}

	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(5);
	ImGui::PopFont();

	// TODO: Move this dropdown to the modal for creating a new project
	// Dropdown for rendering api
	/*ImGui::PushFont(headerFont);
	ImGui::Text("Rendering API");
	ImGui::PopFont();
	ImGui::Separator();


	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
	const char* apis[] = { "OpenGL", "DirectX 11", "DirectX 12", "Vulkan" };
	static int currentApiIndex = 0;
	const char* combo_preview_value = apis[currentApiIndex];
	ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 4);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10,8));
	ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));

	// Coloring the combo preview
	ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(59, 66, 82, 255));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(76, 86, 106, 255));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(255, 0, 0, 255));
	
	// Coloring the selected combo item
	ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(59, 66, 82, 255));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(67, 76, 94, 255));
	//ImGui::PushStyleColor(ImGuiCol_HeaderActive, IM_COL32(1, 0, 0, 1));
	
	// Coloring the combo popup background
	//ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(76, 86, 106, 255));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(46, 52, 64, 255));

	if (ImGui::BeginCombo("", combo_preview_value)) {
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		for (int n = 0; n < IM_ARRAYSIZE(apis); n++)
		{
			const bool is_selected = (currentApiIndex == n);
			draw_list->ChannelsSplit(2);

			ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));
			draw_list->ChannelsSetCurrent(1);

			if (ImGui::Selectable(apis[n], is_selected)) {
				currentApiIndex = n;
				switch (currentApiIndex) {
				case 0:
					selectedApi = RenderingApi::OpenGL;
					break;
				case 1:
					selectedApi = RenderingApi::DX11;
					break;
				case 2:
					selectedApi = RenderingApi::DX12;
					break;
				case 3:
					selectedApi = RenderingApi::Vulkan;
					break;
				}
			}

			ImGui::PopStyleVar(1);
			ImGui::PopStyleColor(3);
			if (ImGui::IsItemHovered()) {
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				draw_list->ChannelsSetCurrent(0);
				ImVec2 p_min = ImGui::GetItemRectMin();
				ImVec2 p_max = ImGui::GetItemRectMax();
				ImU32 rectCol = ImGui::IsMouseDown(ImGuiMouseButton_Left) ? IM_COL32(76, 86, 106, 255) : IM_COL32(67, 76, 94, 255);

				ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, rectCol, 10);
			}
			else if (is_selected) {
				draw_list->ChannelsSetCurrent(0);
				ImVec2 p_min = ImGui::GetItemRectMin();
				ImVec2 p_max = ImGui::GetItemRectMax();
				ImU32 rectCol = IM_COL32(59, 66, 82, 255);

				ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, rectCol, 10);
			}
			
			draw_list->ChannelsMerge();

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		
		ImGui::EndCombo();
	}

	ImGui::PopStyleColor(6);
	ImGui::PopStyleVar(3);

	if (ImGui::IsItemHovered()) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}

	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(3);

	ImGui::PopFont();
	*/
	ImGui::End();
	//ImGui::PopStyleColor(2);
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

	ImGui::Text("An open source project created by flash-miller - 2021");

	ImGui::End();
	ImGui::PopStyleColor(2);
	ImGui::PopFont();
}

void ProjectLauncher::RenderProjectNotFoundModal() {
	// Centering Modal
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	// ==================== Popup Modal ====================
	if (ImGui::BeginPopupModal("Project not found!", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {

		// ==================== Information Text ====================
		std::string informationText = std::string("The project at \n\"") + projectList[selectedProjectId].path + "\"\ncould not be found.\n\nDo you want to remove it from the list?";
		ImGui::Text(informationText.c_str());
		
		ImGui::Separator();

		// ==================== Remove Button ====================
		if (ImGui::Button("Remove", ImVec2(120, 0))) {
			RemoveProjectFromList(selectedProjectId);
			ImGui::CloseCurrentPopup();
		}

		// ==================== Cancel Button ====================
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}
		
		ImGui::EndPopup();
	}
}

void ProjectLauncher::RenderCreateNewProjectModal() {
	// Centering Modal ====================
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	
	// Setting the dimming background color
	ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0.2, 0.2, 0.2 , 0.6));
	
	// Setting the font for the modal window title
	ImGui::PushFont(headerFont);
	
	// ==================== Popup Modal ====================
	if (ImGui::BeginPopupModal("Create new project", NULL, ImGuiWindowFlags_AlwaysAutoResize )) {
		// ==================== Name Input ====================
		static char newProjectName[128] = "New project";
		ImGui::InputText("", newProjectName, IM_ARRAYSIZE(newProjectName));

		ImGui::Separator();

		// ==================== Rendering API Selection Dropdown ====================
		{
			// Rendering Title
			/*ImGui::PushFont(headerFont);
			ImGui::Text("Rendering API");
			ImGui::PopFont();*/
			
			ImGui::Separator();

			// Setting up combo
			const char* apis[] = { "OpenGL", "DirectX 11", "DirectX 12", "Vulkan" };
			static int currentApiIndex = 0;
			const char* combo_preview_value = apis[currentApiIndex];
			
			/*ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
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
			ImGui::PushStyleColor(ImGuiCol_PopupBg, IM_COL32(46, 52, 64, 255));*/

			if (ImGui::BeginCombo("", combo_preview_value)) {
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				
				// Looping through all the combo entries
				for (int n = 0; n < IM_ARRAYSIZE(apis); n++)
				{
					const bool is_selected = (currentApiIndex == n);

					// Selectable settings
					ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));
					
					// For drawing a custom Selectable background, we split the channel
					// Now we can draw the text in the foreground, and the colored, rounded rectangle in the background
					//draw_list->ChannelsSplit(2);
					//draw_list->ChannelsSetCurrent(1);

					// ==================== Rendering Selectable ====================
					/*if (ImGui::Selectable(apis[n], is_selected)) {
						currentApiIndex = n;
						switch (currentApiIndex) {
						case 0:
							selectedApi = RenderingApi::OpenGL;
							break;
						case 1:
							selectedApi = RenderingApi::DX11;
							break;
						case 2:
							selectedApi = RenderingApi::DX12;
							break;
						case 3:
							selectedApi = RenderingApi::Vulkan;
							break;
						}
					}*/

					// Reset Style
					ImGui::PopStyleVar(1);
					ImGui::PopStyleColor(3);
					
					// Drawing the background rectangle
					/*if (ImGui::IsItemHovered()) {
						ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
						draw_list->ChannelsSetCurrent(0);
						ImVec2 p_min = ImGui::GetItemRectMin();
						ImVec2 p_max = ImGui::GetItemRectMax();
						ImU32 rectCol = ImGui::IsMouseDown(ImGuiMouseButton_Left) ? IM_COL32(76, 86, 106, 255) : IM_COL32(67, 76, 94, 255);

						ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, rectCol, 10);
					}
					else if (is_selected) {
						draw_list->ChannelsSetCurrent(0);
						ImVec2 p_min = ImGui::GetItemRectMin();
						ImVec2 p_max = ImGui::GetItemRectMax();
						ImU32 rectCol = IM_COL32(59, 66, 82, 255);

						ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, rectCol, 10);
					}*/

					//draw_list->ChannelsMerge();

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					//if (is_selected)
					//	ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
			//ImGui::PopFont();

			// Use Hand cursor when hovering over the combo
			/*if (ImGui::IsItemHovered()) {
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}*/

			// Reset remaining style attributes
			//ImGui::PopStyleVar(4);
			//ImGui::PopStyleColor(6);
		}

		// ==================== Create Button ====================
		ImGui::PushFont(textFont);
		if (ImGui::Button("Create", ImVec2(120, 0))) {
			RemoveProjectFromList(selectedProjectId);
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopFont();

		// ==================== Cancel Button ====================
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		ImGui::PushFont(textFont);
		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopFont();
		
		ImGui::EndPopup();
	}
	ImGui::PopFont();
	ImGui::PopStyleColor();
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
	// TODO focus on the opened dialog when trying to return to the project launcher window \
	(if that is not the default behaviour, finding out when implementing the dialog in another thread)
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

void ProjectLauncher::LoadProjectList() {
	std::string fileContent = FileHandler::readFile("data/savedProjects.json");
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

				if (jsonObject["projects"][i].contains("renderingApi")) {
					projectToAdd.renderingApi = jsonObject["projects"][i]["renderingApi"];
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
		jsonObject["projects"][i]["renderingApi"] = projectList[i].renderingApi;
	}

	std::string fileContent = jsonObject.dump(4);
	FileHandler::writeToFile("data/savedProjects.json", fileContent);
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
			foundInfo.renderingApi = jsonObject["projectInformation"]["renderingApi"];
			foundInfo.lastOpened = -1;
		}
	}

	return foundInfo;
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
	return p1.renderingApi > p2.renderingApi;
}

bool projectApiReverseComparator(ProjectInformation p1, ProjectInformation p2) {
	return p1.renderingApi < p2.renderingApi;
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