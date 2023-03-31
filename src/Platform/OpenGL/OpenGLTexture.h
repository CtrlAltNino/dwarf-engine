#pragma once

#include <glad/glad.h>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "Core/Rendering/Texture.h"

namespace Dwarf {
    class OpenGLTexture : public Texture {
        private:
            /// @brief The OpenGL texture handle.
            GLuint ID;
        public:
            OpenGLTexture(std::filesystem::path path);
            virtual uintptr_t GetTextureID() override;
    };
}