#pragma once

#include <glad/glad.h>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "Core/Rendering/ITexture.h"

namespace Dwarf {
        
    class OpenGLTexture : public ITexture {
        private:
            /// @brief The OpenGL texture handle.
            GLuint ID;
        public:
            OpenGLTexture(std::filesystem::path path);
            GLuint GetTextureId();
    };
}