#pragma once

#ifdef __APPLE__
    #include "Core/Rendering/MetalTexture.h"
#else
    #include "Platform/OpenGL/OpenGLTexture.h"
#endif

#include <filesystem>
#include <nlohmann/json.hpp>

#include "Core/Base.h"
#include "Utilities/Common.h"

namespace Dwarf {

    /// @brief Utilities for importing textures.
    class TextureImporter {
        public:
             // Builds texture without meta data
            static Ref<ITexture> CreateTexture(std::filesystem::path path) {
                #ifdef WIN32
                    //return nullptr;
                    return CreateRef<OpenGLTexture>(OpenGLTexture(path));
                #elif __linux__
                    return (ITexture)OpenGLTexture(path);
                #elif __APPLE__
                    return (ITexture)MetalTexture(path);
                #endif
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
                    return (ITexture)OpenGLTexture(path);
                #elif __APPLE__
                    return (ITexture)MetalTexture(path);
                #endif

                return id;
            }
    };
}