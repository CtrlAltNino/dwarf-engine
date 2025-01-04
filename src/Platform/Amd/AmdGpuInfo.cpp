#include "AmdGpuInfo.h"
#include <iostream>

namespace Dwarf
{
  AmdGpuInfo::AmdGpuInfo()
  {
    // Initialize the ADLX system
    ADLX_RESULT  res = ADLX_FAIL;
    IADLXSystem* system = nullptr;
    res = ADLXHelper::Initialize(&system);

    if (res == ADLX_OK && system)
    {
      // Get the GPU interface
      IADLXGPUList* gpuList = nullptr;
      res = system->GetGPUList(&gpuList);
      if (res == ADLX_OK && gpuList)
      {
        // Iterate through GPUs
        IADLXGPU* gpu = nullptr;
        for (adlx_uint i = 0; i < gpuList->Size(); ++i)
        {
          res = gpuList->At(i, &gpu);
          if (res == ADLX_OK && gpu)
          {
            // Get GPU metrics
            IADLXGPUMetrics* gpuMetrics = nullptr;
            res = gpu->GetMetrics(&gpuMetrics);
            if (res == ADLX_OK && gpuMetrics)
            {
              adlx_uint64 totalVRAM = 0;
              adlx_uint64 usedVRAM = 0;

              res = gpuMetrics->TotalVRAM(&totalVRAM);
              res = gpuMetrics->UsedVRAM(&usedVRAM);

              std::cout << "GPU: " << gpu->Name() << "\n";
              std::cout << "Total VRAM: " << totalVRAM / (1024 * 1024)
                        << " MB\n";
              std::cout << "Used VRAM: " << usedVRAM / (1024 * 1024) << " MB\n";

              gpuMetrics->Release();
            }
            gpu->Release();
          }
        }
        gpuList->Release();
      }
      ADLXHelper::Terminate();
    }
    else
    {
      std::cerr << "Failed to initialize ADLX.\n";
    }
  }

  AmdGpuInfo::~AmdGpuInfo() {}

  size_t
  AmdGpuInfo::GetUsedVramMb()
  {
  }

  size_t
  AmdGpuInfo::GetTotalVramMb()
  {
  }
}