#include "GpuInfoFactory.h"
#include "Core/Base.h"
#include "Platform/OpenGL/OpenGLUtilities.h"
#include "Platform/Amd/AmdGpuInfo.h"

namespace Dwarf
{
  GpuInfoFactory::GpuInfoFactory(GraphicsApi graphicsApi)
    : m_GraphicsApi(graphicsApi)
  {
  }

  std::unique_ptr<IGpuInfo>
  GpuInfoFactory::Create() const
  {
    switch (m_GraphicsApi)
    {
      using enum GraphicsApi;
      case None: break;
      case D3D12: break;
      case Metal: break;
      case OpenGL:
        {
          std::string deviceInfo = OpenGLUtilities::GetDeviceInfo();
          if (deviceInfo.find("ATI") != std::string::npos ||
              deviceInfo.find("AMD") != std::string::npos)
          {
            return std::unique_ptr<AmdGpuInfo>();
          }
          break;
        }
      case Vulkan: break;
    }

    return nullptr;
  }
}