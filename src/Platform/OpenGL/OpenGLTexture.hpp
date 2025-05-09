#pragma once

#include "Core/Rendering/Texture/ITexture.hpp"
#include "Core/Rendering/VramTracker/IVramTracker.hpp"
#include "Logging/IDwarfLogger.hpp"
#include <glad/glad.h>

namespace Dwarf
{
  class OpenGLTexture : public ITexture
  {
  private:
    /// @brief The OpenGL texture handle.
    GLuint                        mId = 0;
    TextureResolution             mSize;
    std::shared_ptr<IDwarfLogger> mLogger;
    std::shared_ptr<IVramTracker> mVramTracker;
    size_t                        mVramMemory;
    GLuint                        mTextureType;

  public:
    explicit OpenGLTexture(const std::shared_ptr<TextureContainer>& data,
                           std::shared_ptr<IDwarfLogger>            logger,
                           std::shared_ptr<IVramTracker> vramTracker);
    ~OpenGLTexture() override;

    /**
     * @brief Gets the resolution of the texture. A variadic return type
     * depending on the dimensions of the texture.
     *
     * @return Resolution of the texture
     */
    [[nodiscard]] auto
    GetSize() const -> TextureResolution override;

    /**
     * @brief Gets the id of the texture
     *
     * @return Id representing the texture on the GPU
     */
    [[nodiscard]] auto
    GetTextureID() const -> uintptr_t override;

    void
    SetAnisoLevel(uint8_t anisoLevel) override;

    /**
     * @brief Gets the underlying texture type
     *
     * @return GLuint Type of the texture
     */
    [[nodiscard]] auto
    GetType() const -> GLuint;
  };
} // namespace Dwarf