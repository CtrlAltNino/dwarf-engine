#include "pch.h"

#include "Core/Base.h"
#include "GpuInfoFactory.h"
#include "Logging/IDwarfLogger.h"
#include "Platform/OpenGL/OpenGLUtilities.h"

#ifdef _WIN32
#include "Platform/Amd/AmdGpuInfo.h"
#endif

namespace Dwarf
{
  GpuInfoFactory::GpuInfoFactory(GraphicsApi                   graphicsApi,
                                 std::shared_ptr<IDwarfLogger> logger)
    : mGraphicsApi(graphicsApi)
    , mLogger(std::move(logger))
  {
    mLogger->LogDebug(Log("GpuInfoFactory created", "GpuInfoFactory"));
  }

  auto
  GpuInfoFactory::Create() const -> std::unique_ptr<IGpuInfo>
  {
    mLogger->LogDebug(Log("Creating GpuInfo", "GpuInfoFactory"));

    switch (mGraphicsApi)
    {
      using enum GraphicsApi;
      case None:
        mLogger->LogError(Log("Graphics API is not set", "GpuInfoFactory"));
        throw std::runtime_error("Graphics API is not set");
      case Vulkan:
        mLogger->LogError(
          Log("Vulkan API has not been implemented yet", "GpuInfoFactory"));
        throw std::runtime_error("Vulkan API has not been implemented yet");
#if _WIN32
      case D3D12:
        mLogger->LogError(
          Log("Direct3D12 has not been implemented yet", "GpuInfoFactory"));
        throw std::runtime_error("Direct3D12 has not been implemented yet");
      case OpenGL:
        {
          std::string deviceInfo = OpenGLUtilities::GetDeviceInfo();
          if (deviceInfo.find("ATI") != std::string::npos ||
              deviceInfo.find("AMD") != std::string::npos)
          {
            return std::make_unique<AmdGpuInfo>(mLogger);
          }
          break;
        }
#elif __linux__
      case GraphicsApi::D3D12:
        mLogger->LogError(
          Log("Direct3D12 is only supported on Windows", "GpuInfoFactory"));
        throw std::runtime_error("Direct3D12 is only supported on Windows");
      case GraphicsApi::OpenGL: break;
#endif
    }

    mLogger->LogError(Log("Failed to create GpuInfo", "GpuInfoFactory"));

    return nullptr;
  }
}