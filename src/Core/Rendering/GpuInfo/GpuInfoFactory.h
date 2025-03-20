#pragma once

#include "Core/Base.h"
#include "IGpuInfoFactory.h"
#include "Logging/IDwarfLogger.h"
#include <memory>

namespace Dwarf
{
  class GpuInfoFactory : public IGpuInfoFactory
  {
  private:
    GraphicsApi                   mGraphicsApi;
    std::shared_ptr<IDwarfLogger> mLogger;

  public:
    GpuInfoFactory(GraphicsApi                   graphicsApi,
                   std::shared_ptr<IDwarfLogger> logger);
    ~GpuInfoFactory() override = default;

    std::unique_ptr<IGpuInfo>
    Create() const override;
  };
}