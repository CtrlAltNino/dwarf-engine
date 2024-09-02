#pragma once
#include "pch.h"
#include "Logging/IDwarfLogger.h"
#include <glad/glad.h>
#include "Core/Rendering/Texture/ITexture.h"

namespace Dwarf
{
  class OpenGLTexture : public ITexture
  {
  private:
    /// @brief The OpenGL texture handle.
    GLuint                        m_Id;
    TextureResolution             m_Size;
    std::shared_ptr<IDwarfLogger> m_Logger;

  public:
    explicit OpenGLTexture(std::shared_ptr<TextureContainer>  data,
                           std::shared_ptr<IDwarfLogger>      logger,
                           std::shared_ptr<TextureParameters> parameters =
                             std::make_shared<TextureParameters>());
    ~OpenGLTexture() override;

    TextureResolution
    GetSize() const override;

    uintptr_t
    GetTextureID() override;
  };
} // namespace Dwarf