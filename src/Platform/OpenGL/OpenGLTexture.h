#pragma once
#include "Core/Rendering/VramTracker/IVramTracker.h"
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
    std::shared_ptr<IVramTracker> m_VramTracker;
    size_t                        m_VramMemory;

  public:
    explicit OpenGLTexture(std::shared_ptr<TextureContainer> data,
                           std::shared_ptr<IDwarfLogger>     logger,
                           std::shared_ptr<IVramTracker>     vramTracker);
    ~OpenGLTexture() override;

    TextureResolution
    GetSize() const override;

    uintptr_t
    GetTextureID() const override;
  };
} // namespace Dwarf