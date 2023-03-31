#include "dpch.h"

#include "Core/Rendering/Framebuffer.h"
#include "Core/Rendering/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Dwarf {
    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
#ifdef _WIN32
			//case RendererAPI::API::None:    HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case GraphicsApi::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
#elif __linux__
			case GraphicsApi::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
#endif
		}

		//HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}