#include "Editor/Modules/Performance/PerformanceWindow.h"

namespace Dwarf
{

  PerformanceWindow::PerformanceWindow(
    std::shared_ptr<IEditorStats> editorStats)
    : IGuiModule(ModuleLabel("Performance"),
                 ModuleType(MODULE_TYPE::PERFORMANCE),
                 ModuleID(std::make_shared<UUID>()))
    , m_EditorStats(editorStats)
  {
  }

  PerformanceWindow::PerformanceWindow(
    nlohmann::json                serializedModule,
    std::shared_ptr<IEditorStats> editorStats)
    : IGuiModule(ModuleLabel("Performance"),
                 ModuleType(MODULE_TYPE::PERFORMANCE),
                 ModuleID(std::make_shared<UUID>()))
    , m_EditorStats(editorStats)
  {
    Deserialize(serializedModule);
  }

  void
  PerformanceWindow::OnUpdate()
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

    ImGui::Text("%s",
                std::format("Frames per second: {}",
                            std::to_string(1.0 / m_EditorStats->GetDeltaTime()))
                  .c_str());
    ImGui::Text(
      "%s",
      std::format("Frametime: {}{}",
                  std::to_string(m_EditorStats->GetDeltaTime() * 1000.0),
                  " ms")
        .c_str());
    // ImGui::Text(
    //   "%s",
    //   std::format("Render time: {}{}",
    //               std::to_string(m_EditorStats->GetDeltaTime() * 1000.0),
    //               " ms")
    //     .c_str());

    ImGui::End();
  }

  void
  PerformanceWindow::Deserialize(nlohmann::json moduleData)
  {
    // Deserialization of saved data
  }

  nlohmann::json
  PerformanceWindow::Serialize() const
  {
    // TODO: Implement serialization
    return "";
  }
}