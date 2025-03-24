#pragma once
#include "Core/Rendering/GpuInfo/IGpuInfo.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "Editor/Modules/IGuiModule.h"
#include "Editor/Stats/IEditorStats.h"
#include "Logging/IDwarfLogger.h"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  /// @brief Module to render a window that displays performance statistics.
  class PerformanceWindow : public IGuiModule
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    std::shared_ptr<IEditorStats> mEditorStats;
    std::shared_ptr<IRendererApi> mRendererApi;
    std::shared_ptr<IVramTracker> mVramTracker;
    std::unique_ptr<IGpuInfo>     mGpuInfo;

  public:
    PerformanceWindow(std::shared_ptr<IDwarfLogger> logger,
                      std::shared_ptr<IEditorStats> editorStats,
                      std::shared_ptr<IRendererApi> rendererApi,
                      std::shared_ptr<IVramTracker> vramTracker,
                      std::unique_ptr<IGpuInfo>     gpuInfo);

    PerformanceWindow(SerializedModule              serializedModule,
                      std::shared_ptr<IDwarfLogger> logger,
                      std::shared_ptr<IEditorStats> editorStats,
                      std::shared_ptr<IRendererApi> rendererApi,
                      std::shared_ptr<IVramTracker> vramTracker,
                      std::unique_ptr<IGpuInfo>     gpuInfo);

    ~PerformanceWindow() override;

    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    void
    OnUpdate() override;

    auto
    Serialize() -> nlohmann::json override;

    void
    Deserialize(const nlohmann::json& moduleData);
  };
}