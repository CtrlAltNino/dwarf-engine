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
  Ref<ImGuiLayer>
  ImGuiLayer::Create(GraphicsApi api)
  {
#ifdef _WIN32
    switch (api)
    {
      case GraphicsApi::D3D12:
        // return CreateRef<D3D12ImGuiLayer>();
        break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::OpenGL: return CreateRef<OpenGLImGuiLayer>(); break;
      case GraphicsApi::Vulkan:
        // return CreateRef<VulkanImGuiLayer>();
        break;
    }
#elif __linux__
    switch (api)
    {
      case GraphicsApi::D3D12: break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::OpenGL: return CreateRef<OpenGLImGuiLayer>(); break;
      case GraphicsApi::Vulkan:
        // return CreateRef<VulkanImGuiLayer>();
        break;
    }
#elif __APPLE__
    switch (api)
    {
      case GraphicsApi::D3D12: break;
      case GraphicsApi::Metal:
        // return CreateRef<MetalImGuiLayer>();
        break;
      case GraphicsApi::OpenGL: break;
      case GraphicsApi::Vulkan: break;
    }
#endif

    return nullptr;
  }
}