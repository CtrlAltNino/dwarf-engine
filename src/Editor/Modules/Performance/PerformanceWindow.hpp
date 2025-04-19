#pragma once

#include "Core/Rendering/RendererApi/IRendererApi.hpp"
#include "Core/Rendering/VramTracker/IVramTracker.hpp"
#include "Editor/Modules/IGuiModule.hpp"
#include "Editor/Stats/IEditorStats.hpp"
#include "Logging/IDwarfLogger.hpp"
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

  public:
    PerformanceWindow(std::shared_ptr<IDwarfLogger> logger,
                      std::shared_ptr<IEditorStats> editorStats,
                      std::shared_ptr<IRendererApi> rendererApi,
                      std::shared_ptr<IVramTracker> vramTracker);

    PerformanceWindow(SerializedModule              serializedModule,
                      std::shared_ptr<IDwarfLogger> logger,
                      std::shared_ptr<IEditorStats> editorStats,
                      std::shared_ptr<IRendererApi> rendererApi,
                      std::shared_ptr<IVramTracker> vramTracker);

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