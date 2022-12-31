#include "PerformanceModule.h"

PerformanceModule::PerformanceModule(IViewListener *listener, double* frametime, double* renderTime, int id)
    :IModule(listener, "Performance statistics", MODULE_TYPE::PERFORMANCE, id), frametime(frametime), renderTime(renderTime){
		/*std::cout << "Module created with:" << std::endl;
		std::cout << "Name: " << moduleLabel << std::endl;
		std::cout << "Index: " << moduleIndex << std::endl;
		std::cout << "Identifier: " << GetIdentifier() << std::endl;*/
}

void PerformanceModule::RenderModuleWindow(){
    ImGuiWindowFlags window_flags = 0;

	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
	//window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
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

	ImGui::PushFont(IWindowManager::fonts["smallHeaderFont"]);
	ImGui::Text("Renderer information");
	ImGui::PopFont();

	ImGui::Text((std::string("Vendor name: ") + IWindowManager::vendorName).c_str());
	ImGui::Text((std::string("Renderer name: ") + IWindowManager::rendererName).c_str());
	ImGui::Text((std::string("API version: ") + IWindowManager::apiVersion).c_str());

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);

	ImGui::PushFont(IWindowManager::fonts["smallHeaderFont"]);
	ImGui::Text("Statistics");
	ImGui::PopFont();
    
	ImGui::Text(std::string("Frames per second: " + std::to_string(1.0 / *frametime)).c_str());
    ImGui::Text(std::string("Frametime: " + std::to_string(*frametime * 1000.0) + " ms").c_str());
    ImGui::Text(std::string("Render time: " + std::to_string(*renderTime * 1000.0) + " ms").c_str());

    ImGui::End();

	
    if(!windowOpened)
        viewListener->RemoveWindow(moduleIndex);
}