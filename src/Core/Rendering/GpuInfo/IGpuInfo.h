#pragma once

#include <cstddef>
namespace Dwarf
{
  class IGpuInfo
  {
  public:
    virtual ~IGpuInfo() = default;

    virtual size_t
    GetUsedVramMb() = 0;

    virtual size_t
    GetTotalVramMb() = 0;
  };
}