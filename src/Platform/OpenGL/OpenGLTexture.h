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
        explicit OpenGLTexture(std::filesystem::path const &path);
        ~OpenGLTexture() override;
        uintptr_t GetTextureID() override;
    };
}