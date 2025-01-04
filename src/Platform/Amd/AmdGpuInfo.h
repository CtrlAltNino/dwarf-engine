#pragma once

#include "Core/Rendering/GpuInfo/IGpuInfo.h"

namespace Dwarf
{
  class AmdGpuInfo : public IGpuInfo
  {
  public:
    AmdGpuInfo();
    ~AmdGpuInfo() override;

    size_t
    GetUsedVramMb() override;

    size_t
    GetTotalVramMb() override;
  };
}