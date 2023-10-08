#include "dpch.h"
#include <imgui_internal.h>

#include "Editor/Modules/Performance/PerformanceWindow.h"

namespace Dwarf
{

	PerformanceWindow::PerformanceWindow(Ref<EditorModel> model, int id)
		: GuiModule(model, "Performance", MODULE_TYPE::PERFORMANCE, id), m_Frametime(m_Frametime), m_RenderTime(m_RenderTime) {}

	void PerformanceWindow::OnUpdate(double deltaTimte)
	{
	}

	void PerformanceWindow::OnImGuiRender()
	{
		ImGuiWindowFlags window_flags = 0;

		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
		// window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		// window_flags |= ImGuiWindowFlags_NoTitleBar;
		// window_flags |= ImGuiWindowFlags_MenuBar;

		// static bool b_open = true;

		// if (!ImGui::Begin((moduleLabel + "##" + std::to_string(index)).c_str(), NULL, window_flags))
		if (!ImGui::Begin(GetIdentifier().c_str(), &m_WindowOpened, window_flags))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}

		// ImGui::PushFont(IWindowManager::fonts["smallHeaderFont"]);
		ImGui::Text("Renderer information");
		ImGui::PopFont();

		// ImGui::Text((std::string("Vendor name: ") + IWindowManager::vendorName).c_str());
		// ImGui::Text((std::string("Renderer name: ") + IWindowManager::rendererName).c_str());
		// ImGui::Text((std::string("API version: ") + IWindowManager::apiVersion).c_str());

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);

		// ImGui::PushFont(IWindowManager::fonts["smallHeaderFont"]);
		ImGui::Text("Statistics");
		ImGui::PopFont();

		ImGui::Text(std::string("Frames per second: " + std::to_string(1.0 / *m_Frametime)).c_str());
		ImGui::Text(std::string("Frametime: " + std::to_string(*m_Frametime * 1000.0) + " ms").c_str());
		ImGui::Text(std::string("Render time: " + std::to_string(*m_RenderTime * 1000.0) + " ms").c_str());

		ImGui::End();
	}

	std::string PerformanceWindow::Serialize()
	{
		return "";
	}
}