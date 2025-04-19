#pragma once

#include "Core/Rendering/RendererApi/IRendererApiFactory.hpp"
#include "Editor/Modules/Performance/IPerformanceWindowFactory.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class PerformanceWindowFactory : public IPerformanceWindowFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>        mLogger;
    std::shared_ptr<IEditorStats>        mEditorStats;
    std::shared_ptr<IRendererApiFactory> mRendererApiFactory;
    std::shared_ptr<IVramTracker>        mVramTracker;

  public:
    PerformanceWindowFactory(
      std::shared_ptr<IDwarfLogger>        logger,
      std::shared_ptr<IEditorStats>        editorStats,
      std::shared_ptr<IRendererApiFactory> rendererApiFactory,
      std::shared_ptr<IVramTracker>        vramTracker);

    ~PerformanceWindowFactory() override;

    /**
     * @brief Creates a default PerformanceWindow instance
     *
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] auto
    Create() const -> std::unique_ptr<PerformanceWindow> override;

    /**
     * @brief Creates a PerformanceWindow instance based off serialized data
     *
     * @param serializedModule Serialized data of a PerformanceWindow
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] auto
    Create(SerializedModule serializedModule) const
      -> std::unique_ptr<PerformanceWindow> override;
  };
}