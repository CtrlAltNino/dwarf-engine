#include "PerformanceModule.h"

PerformanceModule::PerformanceModule(float* frametime)
    :IModule("Performance statistics"), frametime(frametime){ }

void PerformanceModule::RenderModuleWindow(){
    ImGuiWindowFlags window_flags = 0;

	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;
	//window_flags |= ImGuiWindowFlags_NoCollapse;
	//window_flags |= ImGuiWindowFlags_NoTitleBar;
	//window_flags |= ImGuiWindowFlags_MenuBar;

    if (!ImGui::Begin(moduleName.c_str(), NULL, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

    ImGui::Text(std::string("Frames per second: " + std::to_string(1.0 / *frametime)).c_str());
    ImGui::Text(std::string("Frametime: " + std::to_string(*frametime) + " ms").c_str());

    ImGui::End();
}