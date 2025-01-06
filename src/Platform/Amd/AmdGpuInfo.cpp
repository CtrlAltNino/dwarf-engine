#include "AmdGpuInfo.h"
#include <iostream>

namespace Dwarf
{
  AmdGpuInfo::AmdGpuInfo()
  {
    ADLX_RESULT res = ADLX_FAIL;

    // Initialize ADLX
    res = m_AdlxHelper.Initialize();

    if (ADLX_SUCCEEDED(res))
    {
      // Get Performance Monitoring services
      ADLX_RESULT res =
        m_AdlxHelper.GetSystemServices()->GetPerformanceMonitoringServices(
          &m_PerformanceMonitoringServices);
      if (ADLX_SUCCEEDED(res))
      {
        IADLXGPUListPtr gpus;
        // Get GPU list
        res = m_AdlxHelper.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res))
        {
          // Use the first GPU in the list
          IADLXGPUPtr oneGPU;
          res = gpus->At(gpus->Begin(), &oneGPU);
          if (ADLX_SUCCEEDED(res))
          {
            // Display main menu options
            // MainMenu();
            // Get and execute the choice
            // MenuControl(perfMonitoringService, oneGPU);
            m_Gpu->TotalVRAM(m_TotalVram);
          }
          else
            std::cout << "\\tGet particular GPU failed" << std::endl;
        }
        else
          std::cout << "\\tGet GPU list failed" << std::endl;
      }
      else
        std::cout << "\\tGet performance monitoring services failed"
                  << std::endl;
    }
  }

  AmdGpuInfo::~AmdGpuInfo()
  {
    m_AdlxHelper.Terminate();
  }

  size_t
  AmdGpuInfo::GetUsedVramMb()
  {
    IADLXGPUMetricsSupportPtr gpuMetricsSupport;
    IADLXGPUMetricsPtr        gpuMetrics;
    m_PerformanceMonitoringServices->GetCurrentGPUMetrics(m_Gpu, &gpuMetrics);

    adlx_bool supported = false;
    adlx_int  VRAM = 0;
    // Display GPU VRAM support status
    ADLX_RESULT res = gpuMetricsSupport->IsSupportedGPUVRAM(&supported);
    if (ADLX_SUCCEEDED(res))
    {
      std::cout << "GPU VRAM support status: " << supported << std::endl;
      if (supported)
      {
        res = gpuMetrics->GPUVRAM(&VRAM);
        if (ADLX_SUCCEEDED(res))
          std::cout << "The GPU VRAM is: " << VRAM << "MB" << std::endl;
      }
    }

    return VRAM;
  }

  size_t
  AmdGpuInfo::GetTotalVramMb()
  {
    if (m_TotalVram != nullptr)
    {
      return *m_TotalVram;
    }

    return 0;
  }
}