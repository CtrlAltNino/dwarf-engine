#pragma once
#include "../../../utilities/dpch.h"
#include "../../data structures/Texture.h"
#include "../../data structures/OpenGLTexture.h"

#ifdef __APPLE__
    #include "../../data structures/MetalTexture.h"
#endif

#include <filesystem>
#include <nlohmann/json.hpp>

/// @brief Utilities for importing textures.
class TextureUtilities {
    public:
        /// @brief Imports a texture.
        /// @param path Path to the texture.
        /// @return Imported texture instance.
        static Texture Import(std::string path) {
            Texture imported;

            return imported;
        }

        // Builds texture without meta data
        static ITexture* CreateTexture(std::filesystem::path path) {
            #ifdef WIN32
                return nullptr;
            #elif __linux__

            #elif __APPLE__
                return (ITexture)new MetalTexture(path);
            #endif
        }

        // Builds the texture with given meta data
        static ITexture* CreateTexture(std::filesystem::path path, nlohmann::json metaData) {

        }
};