#include "PerformanceWindowFactory.h"

namespace Dwarf
{
  PerformanceWindowFactory::PerformanceWindowFactory(
    std::shared_ptr<IEditorStats>        editorStats,
    std::shared_ptr<IRendererApiFactory> rendererApiFactory,
    std::shared_ptr<IVramTracker>        vramTracker,
    std::shared_ptr<IGpuInfoFactory>     gpuInfoFactory)
    : m_EditorStats(editorStats)
    , m_RendererApiFactory(rendererApiFactory)
    , m_VramTracker(vramTracker)
    , m_GpuInfoFactory(gpuInfoFactory)
  {
  }

  std::unique_ptr<PerformanceWindow>
  PerformanceWindowFactory::Create() const
  {
    return std::make_unique<PerformanceWindow>(m_EditorStats,
                                               m_RendererApiFactory->Create(),
                                               m_VramTracker,
                                               m_GpuInfoFactory->Create());
  }

  std::unique_ptr<PerformanceWindow>
  PerformanceWindowFactory::Create(SerializedModule serializedModule) const
  {
    return std::make_unique<PerformanceWindow>(serializedModule,
                                               m_EditorStats,
                                               m_RendererApiFactory->Create(),
                                               m_VramTracker,
                                               m_GpuInfoFactory->Create());
  }
} // namespace Dwarf