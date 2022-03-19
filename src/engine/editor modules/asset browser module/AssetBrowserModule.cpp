#include "AssetBrowserModule.h"

AssetBrowserModule::AssetBrowserModule(IViewListener *listener, std::string path, int id)
    :IModule(listener, "Asset browser", MODULE_TYPE::PERFORMANCE, id),
        assetDirectoryPath(path),
        currentDirectory(path){
		/*std::cout << "Module created with:" << std::endl;
		std::cout << "Name: " << moduleLabel << std::endl;
		std::cout << "Index: " << moduleIndex << std::endl;
		std::cout << "Identifier: " << GetIdentifier() << std::endl;*/
}

void AssetBrowserModule::RenderModuleWindow(){
    ImGuiWindowFlags window_flags = 0;

	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;
	//window_flags |= ImGuiWindowFlags_NoCollapse;
	//window_flags |= ImGuiWindowFlags_NoTitleBar;
	//window_flags |= ImGuiWindowFlags_MenuBar;

	//static bool b_open = true;

    //if (!ImGui::Begin((moduleLabel + "##" + std::to_string(index)).c_str(), NULL, window_flags))
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(500,500));
    if (!ImGui::Begin(GetIdentifier().c_str(), &windowOpened, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

    //ImGui::Text(std::string("Frames per second: " + std::to_string(1.0 / *frametime)).c_str());
    //ImGui::Text(std::string("Frametime: " + std::to_string(*frametime) + " ms").c_str());
    //std::cout << assetDirectoryPath << std::endl;

    static float padding = 16.0f;
    static float buttonSize = 128.0f;
    float cellSize = buttonSize + padding;

    float panelWidth = ImGui::GetContentRegionAvailWidth();
    int columnCount = (int)(panelWidth / cellSize);
    if(columnCount < 1) columnCount = 1;

    ImGui::Columns(columnCount, 0, false);
    int id = 0;
    ImGui::SetColumnWidth(-1, cellSize);
    
    for(auto& directoryEntry : std::filesystem::directory_iterator(currentDirectory)){
        const auto& path = directoryEntry.path();
        auto relativePath = std::filesystem::relative(path, assetDirectoryPath);
        std::string filenameString = relativePath.filename().string();

        ImGui::Button("##entry", ImVec2(buttonSize, buttonSize));
        if(ImGui::IsItemHovered()){
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)){
                if(directoryEntry.is_directory()){
                    currentDirectory /= path.filename();
                }
            }
        }else{
            //ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
        }

        ImGui::TextWrapped(filenameString.c_str());

        ImGui::NextColumn();
    }

    ImGui::End();

    ImGui::PopStyleVar();

	
    if(!windowOpened)
        viewListener->RemoveWindow(moduleIndex);
}