#include "dpch.h"

#include "Core/Rendering/RendererApi.h"
#include "Core/Rendering/Renderer.h"
#include "Platform/OpenGL/OpenGLRendererApi.h"
#include "Platform/Direct3D12/D3D12RendererApi.h"
#include "Platform/Vulkan/VulkanRendererApi.h"

namespace Dwarf {
    Ref<RendererApi> RendererApi::Create(){
        switch(Renderer::GetAPI()){
#ifdef _WIN32
            case GraphicsApi::D3D12:
                    return CreateRef<D3D12RendererApi>();
                break;
            case GraphicsApi::OpenGL:
                    return CreateRef<OpenGLRendererApi>();
                break;
            case GraphicsApi::Vulkan:
                    return CreateRef<VulkanRendererApi>();
                break;
#elif __linux__
            case D3D12: break;
            case OpenGL: break;
            case Vulkan: break;
#endif
        }
        return nullptr;
    }
}