#include "Editor/Modules/Performance/PerformanceWindow.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"
#include <algorithm>
#include <fmt/format.h>
#include <cmath>
#include <glm/fwd.hpp>

namespace Dwarf
{

  PerformanceWindow::PerformanceWindow(
    std::shared_ptr<IEditorStats> editorStats,
    std::shared_ptr<IRendererApi> rendererApi,
    std::shared_ptr<IVramTracker> vramTracker,
    std::unique_ptr<IGpuInfo>     gpuInfo)
    : IGuiModule(ModuleLabel("Performance"),
                 ModuleType(MODULE_TYPE::PERFORMANCE),
                 ModuleID(std::make_shared<UUID>()))
    , m_EditorStats(editorStats)
    , m_RendererApi(rendererApi)
    , m_VramTracker(vramTracker)
    , m_GpuInfo(std::move(gpuInfo))
  {
  }

  PerformanceWindow::PerformanceWindow(
    SerializedModule              serializedModule,
    std::shared_ptr<IEditorStats> editorStats,
    std::shared_ptr<IRendererApi> rendererApi,
    std::shared_ptr<IVramTracker> vramTracker,
    std::unique_ptr<IGpuInfo>     gpuInfo)
    : IGuiModule(ModuleLabel("Performance"),
                 ModuleType(MODULE_TYPE::PERFORMANCE),
                 ModuleID(std::make_shared<UUID>(
                   serializedModule.t["id"].get<std::string>())))
    , m_EditorStats(editorStats)
    , m_RendererApi(rendererApi)
    , m_VramTracker(vramTracker)
    , m_GpuInfo(std::move(gpuInfo))
  {
    Deserialize(serializedModule.t);
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

    // Get delta time, add to array, and plot imgui lines
    static std::vector<float> values = { 0 };
    // static int    values_offset = 0;
    values.push_back((float)m_EditorStats->GetDeltaTime());

    if (values.size() > 100)
    {
      values.erase(values.begin());
    }

    float max = 0.0f;
    for (int i = 0; i < values.size(); i++)
    {
      if (values[i] > max)
      {
        max = values[i];
      }
    }

    std::string overlay =
      fmt::format("Frame time: {:.3f} ms", values[values.size() - 1] * 1000.0f);

    ImGui::PlotLines("Frame time",
                     values.data(),
                     values.size(),
                     0,
                     overlay.c_str(),
                     0.0f,
                     max,
                     ImVec2(0, 80));

    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    VRAMUsageBuffer currentVRAMUsage = m_RendererApi->QueryVRAMUsage();

    float progress_saturated =
      std::clamp((float)currentVRAMUsage.usedMemoryMb /
                   (float)currentVRAMUsage.totalMemoryMb,
                 0.0f,
                 1.0f);
    char buf[64];
    sprintf(buf,
            "%d Mb / %d Mb",
            (int)(progress_saturated * currentVRAMUsage.totalMemoryMb),
            currentVRAMUsage.totalMemoryMb);

    ImGui::ProgressBar(progress_saturated, ImVec2(0.f, 0.f), buf);
    ImGui::SameLine();
    ImGui::Text("VRAM Usage");

    if (m_GpuInfo)
    {
      float progress_saturated = std::clamp(
        (float)m_GpuInfo->GetUsedVramMb() / (float)m_GpuInfo->GetTotalVramMb(),
        0.0f,
        1.0f);
      char buf[64];
      sprintf(buf,
              "%d Mb / %zu Mb",
              (int)(progress_saturated * m_GpuInfo->GetTotalVramMb()),
              m_GpuInfo->GetTotalVramMb());

      ImGui::ProgressBar(progress_saturated, ImVec2(0.f, 0.f), buf);
      ImGui::SameLine();
      ImGui::Text("VRAM Usage AMD");
    }

    std::string textureMemoryString =
      fmt::format("{:.2f} Mb",
                  (double)(m_VramTracker->GetTextureMemory() -
                           m_VramTracker->GetFramebufferMemory()) /
                    1024 / 1024);

    std::string framebufferMemoryString = fmt::format(
      "{:.2f} Mb", (double)m_VramTracker->GetFramebufferMemory() / 1024 / 1024);

    std::string bufferMemoryString = fmt::format(
      "{:.2f} Mb", (double)m_VramTracker->GetBufferMemory() / 1024 / 1024);

    std::string computeShaderMemoryString = fmt::format(
      "{:.2f} Mb", (double)m_VramTracker->GetComputeMemory() / 1024 / 1024);

    std::string shaderMemoryString = fmt::format(
      "{:.2f} Mb", (double)m_VramTracker->GetShaderMemory() / 1024 / 1024);

    ImGui::Text("Texture Memory: %s", textureMemoryString.c_str());
    ImGui::Text("Framebuffer Memory: %s", framebufferMemoryString.c_str());
    ImGui::Text("Buffer Memory: %s", bufferMemoryString.c_str());
    ImGui::Text("Compute Shader Memory: %s", computeShaderMemoryString.c_str());
    ImGui::Text("Shader Memory: %s", shaderMemoryString.c_str());

    ImGui::Text("Device information:\n%s",
                m_EditorStats->GetDeviceInfo().c_str());

    ImGui::End();
  }

  void
  PerformanceWindow::Deserialize(nlohmann::json moduleData)
  {
    // Deserialization of saved data
  }

  nlohmann::json
  PerformanceWindow::Serialize()
  {
    nlohmann::json serializedModule;

    serializedModule["id"] = GetUuid()->ToString();
    serializedModule["type"] = static_cast<int>(GetModuleType());
    serializedModule["label"] = GetModuleName();

    return serializedModule;
  }
}