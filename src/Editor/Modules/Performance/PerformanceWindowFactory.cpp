#include "PerformanceWindowFactory.h"

namespace Dwarf
{
  PerformanceWindowFactory::PerformanceWindowFactory(
    std::shared_ptr<IEditorStats>        editorStats,
    std::shared_ptr<IRendererApiFactory> rendererApiFactory,
    std::shared_ptr<IVramTracker>        vramTracker,
    std::shared_ptr<IGpuInfoFactory>     gpuInfoFactory)
    : mEditorStats(editorStats)
    , mRendererApiFactory(rendererApiFactory)
    , mVramTracker(vramTracker)
    , mGpuInfoFactory(gpuInfoFactory)
  {
  }

  std::unique_ptr<PerformanceWindow>
  PerformanceWindowFactory::Create() const
  {
    return std::make_unique<PerformanceWindow>(mEditorStats,
                                               mRendererApiFactory->Create(),
                                               mVramTracker,
                                               mGpuInfoFactory->Create());
  }

  std::unique_ptr<PerformanceWindow>
  PerformanceWindowFactory::Create(SerializedModule serializedModule) const
  {
    return std::make_unique<PerformanceWindow>(serializedModule,
                                               mEditorStats,
                                               mRendererApiFactory->Create(),
                                               mVramTracker,
                                               mGpuInfoFactory->Create());
  }
} // namespace Dwarf