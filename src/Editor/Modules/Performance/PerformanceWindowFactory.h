#pragma once

#include "Core/Rendering/GpuInfo/IGpuInfoFactory.h"
#include "Core/Rendering/RendererApi/IRendererApiFactory.h"
#include "Editor/Modules/Performance/IPerformanceWindowFactory.h"
#include <boost/di.hpp>

namespace Dwarf
{
  class PerformanceWindowFactory : public IPerformanceWindowFactory
  {
  private:
    std::shared_ptr<IEditorStats>        mEditorStats;
    std::shared_ptr<IRendererApiFactory> mRendererApiFactory;
    std::shared_ptr<IVramTracker>        mVramTracker;
    std::shared_ptr<IGpuInfoFactory>     mGpuInfoFactory;

  public:
    BOOST_DI_INJECT(PerformanceWindowFactory,
                    std::shared_ptr<IEditorStats>        editorStats,
                    std::shared_ptr<IRendererApiFactory> rendererApiFactory,
                    std::shared_ptr<IVramTracker>        vramTracker,
                    std::shared_ptr<IGpuInfoFactory>     gpuInfoFactory);

    ~PerformanceWindowFactory() override = default;
    std::unique_ptr<PerformanceWindow>
    Create() const override;

    std::unique_ptr<PerformanceWindow>
    Create(SerializedModule serializedModule) const override;
  };
}