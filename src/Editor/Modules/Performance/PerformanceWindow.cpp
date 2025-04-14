#include "pch.h"

#include "Core/Rendering/RendererApi/IRendererApi.h"
#include "Editor/Modules/Performance/PerformanceWindow.h"

namespace Dwarf
{

  PerformanceWindow::PerformanceWindow(
    std::shared_ptr<IDwarfLogger> logger,
    std::shared_ptr<IEditorStats> editorStats,
    std::shared_ptr<IRendererApi> rendererApi,
    std::shared_ptr<IVramTracker> vramTracker)
    : IGuiModule(ModuleLabel("Performance"),
                 ModuleType(MODULE_TYPE::PERFORMANCE),
                 ModuleID(std::make_shared<UUID>()))
    , mLogger(std::move(logger))
    , mEditorStats(std::move(editorStats))
    , mRendererApi(std::move(rendererApi))
    , mVramTracker(std::move(vramTracker))
  {
    mLogger->LogDebug(Log("PerformanceWindow created", "PerformanceWindow"));
  }

  PerformanceWindow::PerformanceWindow(
    SerializedModule              serializedModule,
    std::shared_ptr<IDwarfLogger> logger,
    std::shared_ptr<IEditorStats> editorStats,
    std::shared_ptr<IRendererApi> rendererApi,
    std::shared_ptr<IVramTracker> vramTracker)
    : IGuiModule(ModuleLabel("Performance"),
                 ModuleType(MODULE_TYPE::PERFORMANCE),
                 ModuleID(std::make_shared<UUID>(
                   serializedModule.t["id"].get<std::string>())))
    , mLogger(std::move(logger))
    , mEditorStats(std::move(editorStats))
    , mRendererApi(std::move(rendererApi))
    , mVramTracker(std::move(vramTracker))
  {
    Deserialize(serializedModule.t);
    mLogger->LogDebug(Log("PerformanceWindow created", "PerformanceWindow"));
  }

  PerformanceWindow::~PerformanceWindow()
  {
    mLogger->LogDebug(Log("PerformanceWindow destroyed", "PerformanceWindow"));
  }

  void
  PerformanceWindow::OnUpdate()
  {
    // Code that needs to be run before render
  }

  void
  PerformanceWindow::OnImGuiRender()
  {
    ImGuiWindowFlags windowFlags = 0;

    windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
    windowFlags |= ImGuiWindowFlags_NoCollapse;
    if (!ImGui::Begin(GetIdentifier().c_str(), &mWindowOpened, windowFlags))
    {
      // Early out if the window is collapsed, as an optimization.
      ImGui::End();
      return;
    }

    // ImGui::PushFont(IWindowManager::fonts["smallHeaderFont"]);
    ImGui::Text("Renderer information");
    // ImGui::PopFont();

    // Get delta time, add to array, and plot imgui lines
    static std::vector<float> values = { 0 };
    // static int    values_offset = 0;
    values.push_back((float)mEditorStats->GetDeltaTime());

    if (values.size() > 100)
    {
      values.erase(values.begin());
    }

    float max = 0.0F;
    for (float value : values)
    {
      max = std::max(value, max);
    }

    std::string overlay =
      fmt::format("Frame time: {:.3f} ms", values[values.size() - 1] * 1000.0F);

    ImGui::PlotLines("Frame time",
                     values.data(),
                     values.size(),
                     0,
                     overlay.c_str(),
                     0.0F,
                     max,
                     ImVec2(0, 80));

    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    VRAMUsageBuffer currentVRAMUsage = mRendererApi->QueryVRAMUsage();

    float progressSaturated =
      std::clamp((float)currentVRAMUsage.usedMemoryMb /
                   (float)currentVRAMUsage.totalMemoryMb,
                 0.0F,
                 1.0F);
    char buf[64];
    sprintf(buf,
            "%d Mb / %d Mb",
            (int)(progressSaturated * currentVRAMUsage.totalMemoryMb),
            currentVRAMUsage.totalMemoryMb);

    ImGui::ProgressBar(progressSaturated, ImVec2(0.f, 0.f), buf);
    ImGui::SameLine();
    ImGui::Text("VRAM Usage");

    std::string textureMemoryString =
      fmt::format("{:.2f} Mb",
                  (double)(mVramTracker->GetTextureMemory() -
                           mVramTracker->GetFramebufferMemory()) /
                    1024 / 1024);

    std::string framebufferMemoryString = fmt::format(
      "{:.2f} Mb", (double)mVramTracker->GetFramebufferMemory() / 1024 / 1024);

    std::string bufferMemoryString = fmt::format(
      "{:.2f} Mb", (double)mVramTracker->GetBufferMemory() / 1024 / 1024);

    std::string computeShaderMemoryString = fmt::format(
      "{:.2f} Mb", (double)mVramTracker->GetComputeMemory() / 1024 / 1024);

    std::string shaderMemoryString = fmt::format(
      "{:.2f} Mb", (double)mVramTracker->GetShaderMemory() / 1024 / 1024);

    ImGui::Text("Texture Memory: %s", textureMemoryString.c_str());
    ImGui::Text("Framebuffer Memory: %s", framebufferMemoryString.c_str());
    ImGui::Text("Buffer Memory: %s", bufferMemoryString.c_str());
    ImGui::Text("Compute Shader Memory: %s", computeShaderMemoryString.c_str());
    ImGui::Text("Shader Memory: %s", shaderMemoryString.c_str());

    ImGui::Text("Device information:\n%s",
                mEditorStats->GetDeviceInfo().c_str());

    ImGui::End();
  }

  void
  PerformanceWindow::Deserialize(const nlohmann::json& moduleData)
  {
    // Deserialization of saved data
  }

  nlohmann::json
  PerformanceWindow::Serialize()
  {
    nlohmann::json serializedModule;

    serializedModule["id"] = GetUuid()->toString();
    serializedModule["type"] = static_cast<int>(GetModuleType());
    serializedModule["label"] = GetModuleName();

    return serializedModule;
  }
}