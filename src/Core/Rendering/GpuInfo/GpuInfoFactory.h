#pragma once

#include "Core/Base.h"
#include "IGpuInfoFactory.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class GpuInfoFactory : public IGpuInfoFactory
  {
  private:
    GraphicsApi                   m_GraphicsApi;
    std::shared_ptr<IDwarfLogger> m_Logger;

  public:
    GpuInfoFactory(GraphicsApi                   graphicsApi,
                   std::shared_ptr<IDwarfLogger> logger);
    ~GpuInfoFactory() override = default;

    std::unique_ptr<IGpuInfo>
    Create() const override;
  };
}