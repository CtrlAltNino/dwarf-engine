#include "ImGuiLayerFactory.h"

#if _WIN32
#include "Platform/OpenGL/OpenGLImGuiLayer.h"
// #include "Platform/Direct3D12/Direct3D12ImGuiLayer.h"
// #include "Platform/Vulkan/Vulkan/ImGuiLayer.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLImGuiLayer.h"
// #include "Platform/Vulkan/Vulkan/ImGuiLayer.h"
#elif __APPLE__
// #include "Platform/Metal/MetalImGuiLayer.h"
#endif

namespace Dwarf
{
  ImGuiLayerFactory::ImGuiLayerFactory(std::shared_ptr<IDwarfLogger> logger,
                                       GraphicsApi                   api,
                                       ImGuiIniFilePath iniFilePath)
    : mLogger(logger)
    , mApi(api)
    , mIniFilePath(iniFilePath)
  {
    mLogger->LogDebug(Log("ImGuiLayerFactory created.", "ImGuiLayerFactory"));
  }

  ImGuiLayerFactory::~ImGuiLayerFactory()
  {
    mLogger->LogDebug(Log("ImGuiLayerFactory destroyed.", "ImGuiLayerFactory"));
  }

  std::unique_ptr<IImGuiLayer>
  ImGuiLayerFactory::Create() const
  {
    mLogger->LogDebug(Log("Creating ImGuiLayer...", "ImGuiLayerFactory"));
    switch (mApi)
    {
      using enum GraphicsApi;
      case None: throw std::runtime_error("Invalid API."); break;
#if _WIN32

      case D3D12:
        // return std::make_shared<D3D12ImGuiLayer>();
        throw std::runtime_error(
          "Direct3D12 API has not been implemented yet.");
        break;
      case Metal:
        throw std::runtime_error("Metal API is not supported on Windows.");
        break;
      case OpenGL:
        {
          mLogger->LogDebug(
            Log("Creating OpenGLImGuiLayer...", "ImGuiLayerFactory"));
          return std::make_unique<OpenGLImGuiLayer>(mLogger, mIniFilePath);
          break;
        }
      case Vulkan:
        // return std::make_shared<VulkanImGuiLayer>();
        throw std::runtime_error(
          "Direct3D12 API has not been implemented yet.");
        break;
#elif __linux__
      case D3D12:
        throw std::runtime_error("Direct3D12 API is not supported on Linux.");
        break;
      case Metal:
        throw std::runtime_error("Metal API is not supported on Linux.");
        break;
      case OpenGL:
        {
          mLogger->LogDebug(
            Log("Creating OpenGLImGuiLayer...", "ImGuiLayerFactory"));
          return std::make_unique<OpenGLImGuiLayer>(mLogger, mIniFilePath);
          break;
        }
      case Vulkan:
        // return std::make_shared<VulkanImGuiLayer>();
        throw std::runtime_error(
          "Direct3D12 API has not been implemented yet.");
        break;
#elif __APPLE__
      case D3D12:
        throw std::runtime_error("Direct3D12 API is not supported on OSX.");
        break;
      case Metal:
        // return std::make_shared<MetalImGuiLayer>();
        throw std::runtime_error("Metal API has not been implemented yet.");
        break;
      case OpenGL: return std::make_shared<OpenGLImGuiLayer>(); break;
      case Vulkan:
        throw std::runtime_error("Direct3D12 API is not supported on OSX.");
        break;
#endif
    }

    mLogger->LogError(Log("Failed to create ImGuiLayer.", "ImGuiLayerFactory"));
    return nullptr;
  }
} // namespace Dwarf