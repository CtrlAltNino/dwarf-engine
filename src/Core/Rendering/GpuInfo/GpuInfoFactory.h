#pragma once

#include "Core/Base.h"
#include "IGpuInfoFactory.h"

namespace Dwarf
{
  class GpuInfoFactory : public IGpuInfoFactory
  {
  private:
    GraphicsApi m_GraphicsApi;

  public:
    GpuInfoFactory(GraphicsApi graphicsApi);
    ~GpuInfoFactory() override = default;

    std::unique_ptr<IGpuInfo>
    Create() const override;
  };
}