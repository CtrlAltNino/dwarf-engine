#include "dpch.h"

#include "Core/Rendering/Renderer.h"

#ifdef WIN32
    #include "Platform/OpenGL/OpenGLTexture.h"
#elif __linux__
    #include "Platform/OpenGL/OpenGLTexture.h"
#endif

#include <nlohmann/json.hpp>

namespace Dwarf {
    Ref<Texture> Texture::Create(std::filesystem::path path) {
        switch(Renderer::GetAPI()){
            #ifdef WIN32
                case GraphicsApi::D3D12: break;
                case GraphicsApi::OpenGL:
                    return CreateRef<OpenGLTexture>(OpenGLTexture(path));
                    break;
                case GraphicsApi::Vulkan:
                    break;
            #elif __linux__
                case GraphicsApi::D3D12: break;
                case GraphicsApi::OpenGL:
                    return CreateRef<OpenGLTexture>(OpenGLTexture(path));
                    break;
                case GraphicsApi::Vulkan: break;
            #endif
        }
        return nullptr;
    }
}