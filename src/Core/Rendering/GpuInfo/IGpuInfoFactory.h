#pragma once

#include "IGpuInfo.h"
#include <memory>

namespace Dwarf
{
  class IGpuInfoFactory
  {
  public:
    virtual ~IGpuInfoFactory() = default;

    virtual std::unique_ptr<IGpuInfo>
    Create() const = 0;
  };
}