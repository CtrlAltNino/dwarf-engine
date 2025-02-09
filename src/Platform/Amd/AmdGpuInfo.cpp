#include "AmdGpuInfo.h"
#include "Logging/IDwarfLogger.h"
#include <iostream>

namespace Dwarf
{
  AmdGpuInfo::AmdGpuInfo(std::shared_ptr<IDwarfLogger> logger)
    : m_Logger(logger)
  {
    m_Logger->LogDebug(Log("Creating AmdGpuInfo", "AmdGpuInfo"));
    ADLX_RESULT res = ADLX_FAIL;

    // Initialize ADLX
    res = m_AdlxHelper.Initialize();
    m_Logger->LogDebug(Log("Initializing ADLX", "AmdGpuInfo"));

    if (ADLX_SUCCEEDED(res))
    {
      m_Logger->LogDebug(Log("Successfully initialized ADLX", "AmdGpuInfo"));
      // Get Performance Monitoring services
      ADLX_RESULT res =
        m_AdlxHelper.GetSystemServices()->GetPerformanceMonitoringServices(
          &m_PerformanceMonitoringServices);
      if (ADLX_SUCCEEDED(res))
      {
        m_Logger->LogDebug(
          Log("Successfully retrieved performance monitoring services",
              "AmdGpuInfo"));
        IADLXGPUListPtr gpus;
        // Get GPU list
        res = m_AdlxHelper.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res))
        {
          m_Logger->LogDebug(
            Log("Successfully retrieved ADLX GPU List", "AmdGpuInfo"));
          // Use the first GPU in the list
          // IADLXGPUPtr oneGPU;
          res = gpus->At(gpus->Begin(), &m_Gpu);
          if (ADLX_SUCCEEDED(res))
          {
            m_Logger->LogDebug(
              Log("Successfully retrieved first GPU", "AmdGpuInfo"));
            // Display main menu options
            // MainMenu();
            // Get and execute the choice
            // MenuControl(perfMonitoringService, oneGPU);
            ADLX_RESULT resy = m_Gpu->TotalVRAM(m_TotalVram);
            if (ADLX_SUCCEEDED(resy))
            {
              m_Logger->LogDebug(
                Log("Successfully retrieved total VRAM pointer", "AmdGpuInfo"));

              m_Logger->LogDebug(
                Log(fmt::format("Total VRAM: %d", *m_TotalVram), "AmdGpuInfo"));
            }
            else
            {
              m_Logger->LogDebug(Log(
                "Failed to retrieve first total VRAM pointer", "AmdGpuInfo"));
            }
          }
          else
          {
            m_Logger->LogDebug(
              Log("Failed to retrieve first GPU", "AmdGpuInfo"));
          }
        }
        else
        {
          m_Logger->LogDebug(
            Log("Failed to retrieve ADLX GPU List", "AmdGpuInfo"));
        }
      }
      else
      {
        m_Logger->LogDebug(Log(
          "Failed to retrieve performance monitoring services", "AmdGpuInfo"));
      }
    }
    else
    {
      m_Logger->LogDebug(Log("Failed to initialize ADLX", "AmdGpuInfo"));
    }
  }

  AmdGpuInfo::~AmdGpuInfo()
  {
    m_Logger->LogDebug(Log("Destroying AmdGpuInfo", "AmdGpuInfo"));

    if (m_PerformanceMonitoringServices)
    {
      m_PerformanceMonitoringServices.Release();
      m_PerformanceMonitoringServices = nullptr;
    }

    if (m_Gpu)
    {
      m_Gpu.Release();
      m_Gpu = nullptr;
    }

    if (m_GpuMetrics)
    {
      m_Gpu->Release();
      m_Gpu = nullptr;
    }

    m_AdlxHelper.Terminate();

    m_Logger->LogDebug(Log("Successfully destroyed AmdGpuInfo", "AmdGpuInfo"));
  }

  size_t
  AmdGpuInfo::GetUsedVramMb()
  {
    IADLXGPUMetricsSupportPtr gpuMetricsSupport;
    IADLXGPUMetricsPtr        gpuMetrics;
    m_PerformanceMonitoringServices->GetCurrentGPUMetrics(m_Gpu, &gpuMetrics);
    m_PerformanceMonitoringServices->GetSupportedGPUMetrics(m_Gpu,
                                                            &gpuMetricsSupport);

    adlx_bool supported = false;
    adlx_int  VRAM = 0;
    // Display GPU VRAM support status
    ADLX_RESULT res = gpuMetricsSupport->IsSupportedGPUVRAM(&supported);
    if (ADLX_SUCCEEDED(res))
    {
      if (supported)
      {
        res = gpuMetrics->GPUVRAM(&VRAM);
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