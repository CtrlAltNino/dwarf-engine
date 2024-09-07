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
  ImGuiLayerFactory::ImGuiLayerFactory(GraphicsApi api)
    : m_Api(api)
  {
  }

  ImGuiLayerFactory::~ImGuiLayerFactory() {}

  std::shared_ptr<IImGuiLayer>
  ImGuiLayerFactory::Create() const
  {
    switch (m_Api)
    {
      using enum GraphicsApi;
#if _WIN32

      case D3D12:
        // return std::make_shared<D3D12ImGuiLayer>();
        throw std::runtime_error(
          "Direct3D12 API has not been implemented yet.");
        break;
      case Metal:
        throw std::runtime_error("Metal API is not supported on Windows.");
        break;
      case OpenGL: return std::make_shared<OpenGLImGuiLayer>(); break;
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
      case OpenGL: return std::make_shared<OpenGLImGuiLayer>(); break;
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

    return nullptr;
  }
} // namespace Dwarf