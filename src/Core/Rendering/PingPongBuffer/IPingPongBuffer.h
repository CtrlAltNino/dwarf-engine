#pragma once

#include "Core/Rendering/Framebuffer/IFramebuffer.h"

namespace Dwarf
{
  class IPingPongBuffer
  {
  public:
    virtual ~IPingPongBuffer() = default;

    /**
     * @brief Resizes the underlying framebuffers
     *
     * @param resolution Resolution to apply
     */
    virtual void
    Resize(glm::ivec2 resolution) = 0;

    /**
     * @brief Swaps the framebuffers
     *
     */
    virtual void
    Swap() = 0;

    /**
     * @brief Returns the framebuffers currently designate to be read from
     *
     * @return Reference to the read framebuffer
     */
    virtual auto
    GetReadFramebuffer() -> std::weak_ptr<IFramebuffer> = 0;

    /**
     * @brief Returns the framebuffers currently designate to write to
     *
     * @return Reference to the write framebuffer
     */
    virtual auto
    GetWriteFramebuffer() -> std::weak_ptr<IFramebuffer> = 0;

    /**
     * @brief Returns the texture of the read framebuffer
     *
     * @return Reference to the color attachment
     */
    [[nodiscard]] virtual auto
    GetReadTexture() const -> std::shared_ptr<ITexture> = 0;

    /**
     * @brief Returns the texture of the write framebuffer
     *
     * @return Reference to the color attachment
     */
    [[nodiscard]] virtual auto
    GetWriteTexture() const -> std::shared_ptr<ITexture> = 0;
  };
}