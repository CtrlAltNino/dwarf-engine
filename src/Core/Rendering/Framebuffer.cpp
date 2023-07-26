#include "dpch.h"

#include "Core/Rendering/Framebuffer.h"
#include "Core/Rendering/Renderer.h"

#ifdef _WIN32
//#include "Platform/Direct3D12/D3D12Framebuffer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
//#include "Platform/Vulkan/VulkanFramebuffer.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLFramebuffer.h"
//#include "Platform/Vulkan/VulkanFramebuffer.h"
#elif __APPLE__
//#include "Platform/Metal/MetalFramebuffer.h"
#endif

namespace Dwarf {
    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
#ifdef _WIN32
			//case GraphicsApi::D3D12:  return CreateRef<D3D12Framebuffer>(spec);
			case GraphicsApi::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
			//case GraphicsApi::Vulkan:  return CreateRef<VulkanFramebuffer>(spec);
#elif __linux__
			case GraphicsApi::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
			//case GraphicsApi::Vulkan:  return CreateRef<VulkanFramebuffer>(spec);
#elif __APPLE__
			//case GraphicsApi::Metal:  return CreateRef<MetalFramebuffer>(spec);
#endif
		}
		return nullptr;
	}
}