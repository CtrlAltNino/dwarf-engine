#pragma once

#include <glad/glad.h>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "Core/Rendering/Texture.h"

namespace Dwarf
{
    class OpenGLTexture : public Texture
    {
    private:
        /// @brief The OpenGL texture handle.
        GLuint m_Id;

    public:
        explicit OpenGLTexture(TextureParameters const &parameters, glm::ivec3 size, void *data);
        ~OpenGLTexture() override;
        uintptr_t GetTextureID() override;
    };
}