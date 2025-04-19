#pragma once

#include "Core/Rendering/Framebuffer/IFramebuffer.hpp"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.hpp"
#include "IPingPongBuffer.hpp"

namespace Dwarf
{
  class PingPongBuffer : public IPingPongBuffer
  {
  private:
    std::shared_ptr<IFramebufferFactory>         mFramebufferFactory;
    FramebufferSpecification                     mSpec;
    std::array<std::shared_ptr<IFramebuffer>, 2> mBuffers;
    bool                                         mHorizontal = false;

  public:
    PingPongBuffer(std::shared_ptr<IFramebufferFactory> framebufferFactory,
                   const FramebufferSpecification&      spec);
    ~PingPongBuffer() override = default;

    /**
     * @brief Resizes the underlying framebuffers
     *
     * @param resolution Resolution to apply
     */
    void
    Resize(glm::ivec2 resolution) override;

    /**
     * @brief Swaps the framebuffers
     *
     */
    void
    Swap() override;

    /**
     * @brief Returns the framebuffers currently designate to be read from
     *
     * @return Reference to the read framebuffer
     */
    auto
    GetReadFramebuffer() -> std::weak_ptr<IFramebuffer> override;

    /**
     * @brief Returns the framebuffers currently designate to write to
     *
     * @return Reference to the write framebuffer
     */
    auto
    GetWriteFramebuffer() -> std::weak_ptr<IFramebuffer> override;

    /**
     * @brief Returns the texture of the read framebuffer
     *
     * @return Reference to the color attachment
     */
    [[nodiscard]] auto
    GetReadTexture() const -> std::shared_ptr<ITexture> override;

    /**
     * @brief Returns the texture of the write framebuffer
     *
     * @return Reference to the color attachment
     */
    [[nodiscard]] auto
    GetWriteTexture() const -> std::shared_ptr<ITexture> override;
  };
}