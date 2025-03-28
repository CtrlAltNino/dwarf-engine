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
  }

  auto
  GpuInfoFactory::Create() const -> std::unique_ptr<IGpuInfo>
  {
    switch (mGraphicsApi)
    {
      using enum GraphicsApi;
      case None: break;
#if _WIN32
      case D3D12: break;
      case Metal: break;
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
      case Vulkan: break;
#elif __linux__
      case GraphicsApi::D3D12:
        std::runtime_error("Vulkan API has not been implemented yet.");
        break;
      case GraphicsApi::Metal:
        std::runtime_error("Vulkan API has not been implemented yet.");
        break;
      case GraphicsApi::OpenGL:
        {
          break;
        }
      case GraphicsApi::Vulkan:
        std::runtime_error("Vulkan API has not been implemented yet.");
        break;
#elif __APPLE__
      case D3D12: break;
      case Metal: break;
      case OpenGL: break;
      case Vulkan: break;
#endif
    }

    return nullptr;
  }
}