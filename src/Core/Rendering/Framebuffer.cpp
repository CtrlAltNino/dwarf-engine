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
			case GraphicsApi::D3D12:
					//return CreateRef<D3D12Framebuffer>(spec);
				break;
			case GraphicsApi::Metal: break;
			case GraphicsApi::OpenGL:
					return CreateRef<OpenGLFramebuffer>(spec);
				break;
			case GraphicsApi::Vulkan:
					//return CreateRef<VulkanFramebuffer>(spec);
				break;
#elif __linux__
			case GraphicsApi::D3D12: break;
			case GraphicsApi::Metal: break;
			case GraphicsApi::OpenGL:
					return CreateRef<OpenGLFramebuffer>(spec);
				break;
			case GraphicsApi::Vulkan:
					//return CreateRef<VulkanFramebuffer>(spec);
				break;
#elif __APPLE__
			case GraphicsApi::D3D12: break;
			case GraphicsApi::Metal:
					//return CreateRef<MetalFramebuffer>(spec);
				break;
			case GraphicsApi::OpenGL: break;
			case GraphicsApi::Vulkan: break;
#endif
		}
		return nullptr;
	}
}