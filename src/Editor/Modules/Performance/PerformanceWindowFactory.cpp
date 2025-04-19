#include "pch.hpp"

#include "PerformanceWindowFactory.hpp"

namespace Dwarf
{
  PerformanceWindowFactory::PerformanceWindowFactory(
    std::shared_ptr<IDwarfLogger>        logger,
    std::shared_ptr<IEditorStats>        editorStats,
    std::shared_ptr<IRendererApiFactory> rendererApiFactory,
    std::shared_ptr<IVramTracker>        vramTracker)
    : mLogger(std::move(logger))
    , mEditorStats(std::move(editorStats))
    , mRendererApiFactory(std::move(rendererApiFactory))
    , mVramTracker(std::move(vramTracker))
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
    return std::make_unique<PerformanceWindow>(
      mLogger, mEditorStats, mRendererApiFactory->Create(), mVramTracker);
  }

  auto
  PerformanceWindowFactory::Create(SerializedModule serializedModule) const
    -> std::unique_ptr<PerformanceWindow>
  {
    return std::make_unique<PerformanceWindow>(serializedModule,
                                               mLogger,
                                               mEditorStats,
                                               mRendererApiFactory->Create(),
                                               mVramTracker);
  }
} // namespace Dwarf