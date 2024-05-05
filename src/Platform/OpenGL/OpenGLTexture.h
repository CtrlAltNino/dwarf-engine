#pragma once

#include <filesystem>
#include <glad/glad.h>
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
    explicit OpenGLTexture(std::shared_ptr<TextureContainer>  data,
                           std::shared_ptr<TextureParameters> parameters =
                             std::make_shared<TextureParameters>());
    ~OpenGLTexture() override;
    uintptr_t
    GetTextureID() override;
  };
} // namespace Dwarf