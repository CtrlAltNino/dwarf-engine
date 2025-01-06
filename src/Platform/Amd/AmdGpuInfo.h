#pragma once

#include "Core/Rendering/GpuInfo/IGpuInfo.h"
#include <ADLX/SDK/Include/ADLX.h>
#include <ADLX/SDK/Include/ADLXDefines.h>
#include <ADLX/SDK/Include/ADLXStructures.h>
#include <ADLX/SDK/Include/ADLXVersion.h>
#include <ADLX/SDK/ADLXHelper/Windows/Cpp/ADLXHelper.h>
#include <ADLX/SDK/Include/IPerformanceMonitoring.h>
#include <ADLX/SDK/Include/IPerformanceMonitoring2.h>

namespace Dwarf
{
  class AmdGpuInfo : public IGpuInfo
  {
  private:
    ADLXHelper                            m_AdlxHelper;
    IADLXPerformanceMonitoringServicesPtr m_PerformanceMonitoringServices;
    IADLXGPUPtr                           m_Gpu;

  public:
    AmdGpuInfo();
    ~AmdGpuInfo() override;

    size_t
    GetUsedVramMb() override;

    size_t
    GetTotalVramMb() override;
  };
}