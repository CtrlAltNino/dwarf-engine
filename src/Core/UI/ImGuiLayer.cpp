#include "Core/UI/ImGuiLayer.h"
#ifdef _WIN32
// #include "Platform/Direct3D12/D3D12ImGuiLayer.h"
#include "Platform/OpenGL/OpenGLImGuiLayer.h"
// #include "Platform/Vulkan/VulkanImGuiLayer.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLImGuiLayer.h"
// #include "Platform/Vulkan/VulkanImGuiLayer.h"
#elif __APPLE__
// #include "Platform/Metal/MetalImGuiLayer.h"
#endif

namespace Dwarf
{
  std::shared_ptr<ImGuiLayer>
  ImGuiLayer::Create(GraphicsApi api)
  {
#ifdef _WIN32
    switch (api)
    {
      case GraphicsApi::D3D12:
        // return std::make_shared<D3D12ImGuiLayer>();
        break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::OpenGL:
        return std::make_shared<OpenGLImGuiLayer>();
        break;
      case GraphicsApi::Vulkan:
        // return std::make_shared<VulkanImGuiLayer>();
        break;
    }
#elif __linux__
    switch (api)
    {
      case GraphicsApi::D3D12: break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::OpenGL:
        return std::make_shared<OpenGLImGuiLayer>();
        break;
      case GraphicsApi::Vulkan:
        // return std::make_shared<VulkanImGuiLayer>();
        break;
    }
#elif __APPLE__
    switch (api)
    {
      case GraphicsApi::D3D12: break;
      case GraphicsApi::Metal:
        // return std::make_shared<MetalImGuiLayer>();
        break;
      case GraphicsApi::OpenGL: break;
      case GraphicsApi::Vulkan: break;
    }
#endif

    return nullptr;
  }
}