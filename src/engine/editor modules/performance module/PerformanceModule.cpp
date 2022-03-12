#include "PerformanceModule.h"

PerformanceModule::PerformanceModule(IViewListener *listener, float* frametime, int id)
    :IModule(listener, "Performance statistics", MODULE_TYPE::PERFORMANCE, id), frametime(frametime){
		std::cout << "Module created with:" << std::endl;
		std::cout << "Name: " << moduleLabel << std::endl;
		std::cout << "Index: " << moduleIndex << std::endl;
		std::cout << "Identifier: " << GetIdentifier() << std::endl;
}

void PerformanceModule::RenderModuleWindow(){
    ImGuiWindowFlags window_flags = 0;

	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;
	//window_flags |= ImGuiWindowFlags_NoCollapse;
	//window_flags |= ImGuiWindowFlags_NoTitleBar;
	//window_flags |= ImGuiWindowFlags_MenuBar;

	//static bool b_open = true;

    //if (!ImGui::Begin((moduleLabel + "##" + std::to_string(index)).c_str(), NULL, window_flags))
    if (!ImGui::Begin(GetIdentifier().c_str(), &windowOpened, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

    ImGui::Text(std::string("Frames per second: " + std::to_string(1.0 / *frametime)).c_str());
    ImGui::Text(std::string("Frametime: " + std::to_string(*frametime) + " ms").c_str());

    ImGui::End();

	
    if(!windowOpened)
        viewListener->RemoveWindow(moduleIndex);
}