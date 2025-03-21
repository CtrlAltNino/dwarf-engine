#pragma once

#include "IGpuInfo.h"
#include <memory>

namespace Dwarf
{
  /**
   * @brief A class that creates a GPU info object based on the vendor
   *
   */
  class IGpuInfoFactory
  {
  public:
    virtual ~IGpuInfoFactory() = default;

    [[nodiscard]] virtual auto
    Create() const -> std::unique_ptr<IGpuInfo> = 0;
  };
}