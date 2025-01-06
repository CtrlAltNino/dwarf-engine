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
    // Get system metrics support
    IADLXSystemMetricsSupportPtr systemMetricsSupport;
    ADLX_RESULT                  res2 =
      m_PerformanceMonitoringServices->GetSupportedSystemMetrics(
        &systemMetricsSupport);
    // Get GPU metrics support
    IADLXGPUMetricsSupportPtr gpuMetricsSupport;
    res2 = m_PerformanceMonitoringServices->GetSupportedGPUMetrics(
      m_Gpu, &gpuMetricsSupport);

    // Loop 10 time to accumulate data and show the current metrics in each loop
    IADLXAllMetricsPtr    allMetrics;
    IADLXSystemMetricsPtr systemMetrics;
    IADLXGPUMetricsPtr    gpuMetrics;
    IADLXFPSPtr           oneFPS;

    // Get current All metrics
    ADLX_RESULT res1 =
      m_PerformanceMonitoringServices->GetCurrentAllMetrics(&allMetrics);

    // m_Gpu->QueryInterface(const wchar_t* interfaceId, void** ppInterface)

    // adlx::IADLXGPUMemoryPtr gpuMemory;
    if (m_Gpu->QueryInterface(adlxIID_IADLXGPUInfo(), (void**)&gpuMemory) !=
          ADLX_OK ||
        !gpuMemory)
    {
      std::cerr << "Failed to retrieve GPU memory information." << std::endl;
      adlxHelper.Terminate();
      return

        if (ADLX_SUCCEEDED(res1))
      {
        std::cout << "The current all metrics: " << std::endl;
        // Get current system metrics.
        res1 = allMetrics->GetSystemMetrics(&systemMetrics);
        // Get current GPU metrics
        res1 = allMetrics->GetGPUMetrics(m_Gpu, &gpuMetrics);
        // Show timestamp and GPU metrics
        if (ADLX_SUCCEEDED(res1) && ADLX_SUCCEEDED(res2))
        {
          std::cout << std::boolalpha; // display bool variable as true of false

          adlx_bool supported = false;
          // Get if the GPU VRAM is supported
          ADLX_RESULT res = gpuMetricsSupport->IsSupportedGPUVRAM(&supported);
          if (ADLX_SUCCEEDED(res))
          {
            std::cout << "Get if the GPU VRAM is supported: " << supported
                      << std::endl;
            if (supported)
            {
              adlx_int VRAM = 0;
              res = gpuMetrics->GPUVRAM(&VRAM);
              if (ADLX_SUCCEEDED(res)) return VRAM;
            }
          }

          std::cout << std::noboolalpha;
        }
      }

      return 0;
    }

    size_t AmdGpuInfo::GetTotalVramMb() {}
  }