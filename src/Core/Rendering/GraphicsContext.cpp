#include "dpch.h"

#include "Core/Rendering/GraphicsContext.h"
#include "Core/Rendering/Renderer.h"
#include "SDL2/SDL.h"

#ifdef _WIN32
//#include "Platform/OpenGL/D3D11Context.h" - NOT SUPPORTED YET
//#include "Platform/OpenGL/D3D12Context.h" - NOT SUPPORTED YET
#include "Platform/OpenGL/OpenGLContext.h"
//#include "Platform/OpenGL/VulkanContext.h" - NOT SUPPORTED YET
#elif __linux__
#include "Platform/OpenGL/OpenGLContext.h"
//#include "Platform/Vulkan/VulkanContext.h" - NOT SUPPORTED YET
#elif __APPLE__
//#include "Platform/Metal/MetalContext.h" - NOT SUPPORTED YET
#endif

namespace Dwarf {
    Scope<GraphicsContext> GraphicsContext::Create (void* window) {
        switch(Renderer::GetAPI()){
#ifdef _WIN32
            //case GraphicsApi::D3D11: return CreateScope<D3D11Context>(static_cast<SDL_Window*>(window)); - NOT SUPPORTED YET
            //case GraphicsApi::D3D12: return CreateScope<D3D12Context>(static_cast<SDL_Window*>(window)); - NOT SUPPORTED YET
            case GraphicsApi::OpenGL: return CreateScope<OpenGLContext>(static_cast<SDL_Window*>(window));
            //case GraphicsApi::Vulkan: return CreateScope<VulkanContext>(static_cast<SDL_Window*>(window)); - NOT SUPPORTED YET
#elif __linux__
            case GraphicsApi::OpenGL: return CreateScope<OpenGLContext>(static_cast<SDL_Window*>(window));
            //case GraphicsApi::Vulkan: return CreateScope<VulkanContext>(static_cast<SDL_Window*>(window)); - NOT SUPPORTED YET
#elif __APPLE__
            //case GraphicsApi::Metal: return CreateScope<MetalContext>(static_cast<SDL_Window*>(window)); - NOT SUPPORTED YET
#endif
        }
        return nullptr;
    }
}
