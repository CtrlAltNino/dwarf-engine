#pragma once
#include "Core/Rendering/Texture/ITexture.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "Logging/IDwarfLogger.h"
#include "pch.h"
#include <glad/glad.h>

namespace Dwarf
{
  class OpenGLTexture : public ITexture
  {
  private:
    /// @brief The OpenGL texture handle.
    GLuint                        mId;
    TextureResolution             mSize;
    std::shared_ptr<IDwarfLogger> mLogger;
    std::shared_ptr<IVramTracker> mVramTracker;
    size_t                        mVramMemory;

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