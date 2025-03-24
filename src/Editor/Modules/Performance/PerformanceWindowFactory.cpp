#include "pch.h"

#include "PerformanceWindowFactory.h"

namespace Dwarf
{
  PerformanceWindowFactory::PerformanceWindowFactory(
    std::shared_ptr<IDwarfLogger>        logger,
    std::shared_ptr<IEditorStats>        editorStats,
    std::shared_ptr<IRendererApiFactory> rendererApiFactory,
    std::shared_ptr<IVramTracker>        vramTracker,
    std::shared_ptr<IGpuInfoFactory>     gpuInfoFactory)
    : mLogger(std::move(logger))
    , mEditorStats(std::move(editorStats))
    , mRendererApiFactory(std::move(rendererApiFactory))
    , mVramTracker(std::move(vramTracker))
    , mGpuInfoFactory(std::move(gpuInfoFactory))
  {
    mLogger->LogDebug(
      Log("PerformanceWindowFactory created", "PerformanceWindowFactory"));
  }
  PerformanceWindowFactory::~PerformanceWindowFactory()
  {
    mLogger->LogDebug(
      Log("PerformanceWindowFactory destroyed", "PerformanceWindowFactory"));
  }

  auto
  PerformanceWindowFactory::Create() const -> std::unique_ptr<PerformanceWindow>
  {
    return std::make_unique<PerformanceWindow>(mLogger,
                                               mEditorStats,
                                               mRendererApiFactory->Create(),
                                               mVramTracker,
                                               mGpuInfoFactory->Create());
  }

  auto
  PerformanceWindowFactory::Create(SerializedModule serializedModule) const
    -> std::unique_ptr<PerformanceWindow>
  {
    return std::make_unique<PerformanceWindow>(serializedModule,
                                               mLogger,
                                               mEditorStats,
                                               mRendererApiFactory->Create(),
                                               mVramTracker,
                                               mGpuInfoFactory->Create());
  }
} // namespace Dwarf