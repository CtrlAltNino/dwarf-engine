#pragma once

#ifdef WIN32
    #include "Platform/OpenGL/OpenGLTexture.h"
#elif __linux__
    #include "Platform/OpenGL/OpenGLTexture.h"
#elif __APPLE__
    #include "Platform/Metal/MetalTexture.h"
#endif

#include <filesystem>
#include <nlohmann/json.hpp>

#include "Core/Base.h"
#include "Utilities/Common.h"
#include "Core/Rendering/Renderer.h"

namespace Dwarf {

    /// @brief Utilities for importing textures.
    class TextureImporter {
        public:
             // Builds texture without meta data
            static Ref<ITexture> CreateTexture(std::filesystem::path path) {
                switch(Renderer::GetAPI()){
                    #ifdef WIN32
                        case GraphicsApi::D3D11:
                            return nullptr;
                            break;
                        case GraphicsApi::D3D12:
                            return nullptr;
                            break;
                        case GraphicsApi::Metal:
                            return nullptr;
                            break;
                        case GraphicsApi::OpenGL:
                            return CreateRef<OpenGLTexture>(OpenGLTexture(path));
                            break;
                        case GraphicsApi::Vulkan:
                            return nullptr;
                            break;
                    #elif __linux__
                        case GraphicsApi::D3D11:
                            return nullptr;
                            break;
                        case GraphicsApi::D3D12:
                            return nullptr;
                            break;
                        case GraphicsApi::Metal:
                            return nullptr;
                            break;
                        case GraphicsApi::OpenGL:
                            return CreateRef<OpenGLTexture>(OpenGLTexture(path));
                            break;
                        case GraphicsApi::Vulkan:
                            return nullptr;
                            break;
                    #elif __APPLE__
                        case GraphicsApi::D3D11:
                            return nullptr;
                            break;
                        case GraphicsApi::D3D12:
                            return nullptr;
                            break;
                        case GraphicsApi::Metal:
                            return CreateRef<MetalTexture>(path);
                            break;
                        case GraphicsApi::OpenGL:
                            return nullptr;
                            break;
                        case GraphicsApi::Vulkan:
                            return nullptr;
                            break;
                    #endif
                }
            }

            static uintptr_t GetTexID(Ref<ITexture> texture, GraphicsApi api){
                uintptr_t id;
                #ifdef WIN32
                    switch(api){
                        case GraphicsApi::D3D11: break;
                        case GraphicsApi::D3D12: break;
                        case GraphicsApi::OpenGL:
                            id = ((OpenGLTexture*)texture.get())->GetTextureId();
                            break;
                        case GraphicsApi::Vulkan: break;
                    }
                #elif __linux__
                    
                #elif __APPLE__
                    //id = ((MetalTexture*)texture.get())->GetMetalTexture();
                #endif

                return id;
            }
    };
}