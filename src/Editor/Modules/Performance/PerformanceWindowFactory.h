#pragma once

#include "Core/Rendering/GpuInfo/IGpuInfoFactory.h"
#include "Core/Rendering/RendererApi/IRendererApiFactory.h"
#include "Editor/Modules/Performance/IPerformanceWindowFactory.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class PerformanceWindowFactory : public IPerformanceWindowFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>        mLogger;
    std::shared_ptr<IEditorStats>        mEditorStats;
    std::shared_ptr<IRendererApiFactory> mRendererApiFactory;
    std::shared_ptr<IVramTracker>        mVramTracker;
    std::shared_ptr<IGpuInfoFactory>     mGpuInfoFactory;

  public:
    PerformanceWindowFactory(
      std::shared_ptr<IDwarfLogger>        logger,
      std::shared_ptr<IEditorStats>        editorStats,
      std::shared_ptr<IRendererApiFactory> rendererApiFactory,
      std::shared_ptr<IVramTracker>        vramTracker,
      std::shared_ptr<IGpuInfoFactory>     gpuInfoFactory);

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