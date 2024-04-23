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
    explicit OpenGLTexture(
      Ref<TextureContainer>  data,
      Ref<TextureParameters> parameters = CreateRef<TextureParameters>());
    ~OpenGLTexture() override;
    uintptr_t
    GetTextureID() override;
  };
} // namespace Dwarf