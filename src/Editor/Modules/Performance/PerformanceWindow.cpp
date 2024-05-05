#include "dpch.h"
#include <imgui_internal.h>

#include "Editor/Modules/Performance/PerformanceWindow.h"

namespace Dwarf
{

  PerformanceWindow::PerformanceWindow(std::shared_ptr<EditorModel> model,
                                       int                          id)
    : GuiModule(model, "Performance", MODULE_TYPE::PERFORMANCE, id)
  {
  }

  void
  PerformanceWindow::OnUpdate(double deltaTimte)
  {
    // Code that needs to be run before render
  }

  void
  PerformanceWindow::OnImGuiRender()
  {
    ImGuiWindowFlags window_flags = 0;

    window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    if (!ImGui::Begin(GetIdentifier().c_str(), &m_WindowOpened, window_flags))
    {
      // Early out if the window is collapsed, as an optimization.
      ImGui::End();
      return;
    }

    // ImGui::PushFont(IWindowManager::fonts["smallHeaderFont"]);
    ImGui::Text("Renderer information");
    // ImGui::PopFont();

    // ImGui::Text((std::string("Vendor name: ") +
    // IWindowManager::vendorName).c_str()); ImGui::Text((std::string("Renderer
    // name: ") + IWindowManager::rendererName).c_str());
    // ImGui::Text((std::string("API version: ") +
    // IWindowManager::apiVersion).c_str());

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);

    // ImGui::PushFont(IWindowManager::fonts["smallHeaderFont"]);
    ImGui::Text("Statistics");
    // ImGui::PopFont();

    ImGui::Text(
      "%s",
      std::format("Frames per second: {}", std::to_string(1.0 / *m_Frametime))
        .c_str());
    ImGui::Text("%s",
                std::format("Frametime: {}{}",
                            std::to_string(*m_Frametime * 1000.0),
                            " ms")
                  .c_str());
    ImGui::Text("%s",
                std::format("Render time: {}{}",
                            std::to_string(*m_RenderTime * 1000.0),
                            " ms")
                  .c_str());

    ImGui::End();
  }

  void
  PerformanceWindow::Deserialize(nlohmann::json moduleData)
  {
    // Deserialization of saved data
  }

  std::string
  PerformanceWindow::Serialize()
  {
    return "";
  }
}