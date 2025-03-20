#include "AmdGpuInfo.h"
#include "Logging/IDwarfLogger.h"
#include <fmt/format.h>

namespace Dwarf
{
  AmdGpuInfo::AmdGpuInfo(std::shared_ptr<IDwarfLogger> logger)
    : mLogger(logger)
  {
    mLogger->LogDebug(Log("Creating AmdGpuInfo", "AmdGpuInfo"));
    ADLX_RESULT res = ADLX_FAIL;

    // Initialize ADLX
    res = mAdlxHelper.Initialize();
    mLogger->LogDebug(Log("Initializing ADLX", "AmdGpuInfo"));

    if (ADLX_SUCCEEDED(res))
    {
      mLogger->LogDebug(Log("Successfully initialized ADLX", "AmdGpuInfo"));
      // Get Performance Monitoring services
      ADLX_RESULT res =
        mAdlxHelper.GetSystemServices()->GetPerformanceMonitoringServices(
          &mPerformanceMonitoringServices);
      if (ADLX_SUCCEEDED(res))
      {
        mLogger->LogDebug(
          Log("Successfully retrieved performance monitoring services",
              "AmdGpuInfo"));
        IADLXGPUListPtr gpus;
        // Get GPU list
        res = mAdlxHelper.GetSystemServices()->GetGPUs(&gpus);
        if (ADLX_SUCCEEDED(res))
        {
          mLogger->LogDebug(
            Log("Successfully retrieved ADLX GPU List", "AmdGpuInfo"));
          // Use the first GPU in the list
          // IADLXGPUPtr oneGPU;
          res = gpus->At(gpus->Begin(), &mGpu);
          if (ADLX_SUCCEEDED(res))
          {
            mLogger->LogDebug(
              Log("Successfully retrieved first GPU", "AmdGpuInfo"));
            // Display main menu options
            // MainMenu();
            // Get and execute the choice
            // MenuControl(perfMonitoringService, oneGPU);
            ADLX_RESULT resy = mGpu->TotalVRAM(mTotalVram);
            if (ADLX_SUCCEEDED(resy))
            {
              mLogger->LogDebug(
                Log("Successfully retrieved total VRAM pointer", "AmdGpuInfo"));

              mLogger->LogDebug(
                Log(fmt::format("Total VRAM: %d", *mTotalVram), "AmdGpuInfo"));
            }
            else
            {
              mLogger->LogDebug(Log(
                "Failed to retrieve first total VRAM pointer", "AmdGpuInfo"));
            }
          }
          else
          {
            mLogger->LogDebug(
              Log("Failed to retrieve first GPU", "AmdGpuInfo"));
          }
        }
        else
        {
          mLogger->LogDebug(
            Log("Failed to retrieve ADLX GPU List", "AmdGpuInfo"));
        }
      }
      else
      {
        mLogger->LogDebug(Log(
          "Failed to retrieve performance monitoring services", "AmdGpuInfo"));
      }
    }
    else
    {
      mLogger->LogDebug(Log("Failed to initialize ADLX", "AmdGpuInfo"));
    }
  }

  AmdGpuInfo::~AmdGpuInfo()
  {
    mLogger->LogDebug(Log("Destroying AmdGpuInfo", "AmdGpuInfo"));

    if (mPerformanceMonitoringServices)
    {
      mPerformanceMonitoringServices.Release();
      mPerformanceMonitoringServices = nullptr;
    }

    if (mGpu)
    {
      mGpu.Release();
      mGpu = nullptr;
    }

    if (mGpuMetrics)
    {
      mGpu->Release();
      mGpu = nullptr;
    }

    mAdlxHelper.Terminate();

    mLogger->LogDebug(Log("Successfully destroyed AmdGpuInfo", "AmdGpuInfo"));
  }

  size_t
  AmdGpuInfo::GetUsedVramMb()
  {
    IADLXGPUMetricsSupportPtr gpuMetricsSupport;
    IADLXGPUMetricsPtr        gpuMetrics;
    mPerformanceMonitoringServices->GetCurrentGPUMetrics(mGpu, &gpuMetrics);
    mPerformanceMonitoringServices->GetSupportedGPUMetrics(mGpu,
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
    if (mTotalVram != nullptr)
    {
      return *mTotalVram;
    }

    return 0;
  }
}