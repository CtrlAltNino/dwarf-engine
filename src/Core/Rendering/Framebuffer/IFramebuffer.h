#pragma once

#include "Core/Rendering/Texture/ITexture.h"

namespace Dwarf
{
  // TODO: Align with TextureCommon.h
  enum class FramebufferTextureFormat
  {
    None = 0,
    // Color
    RGBA8 = 1,
    SRGBA8 = 2,
    RGBA16F = 3,
    RED_INTEGER = 4,
    // Depth/stencil
    DEPTH24STENCIL8 = 5,
    DEPTH = 6,
    STENCIL = 7
  };

  struct FramebufferTextureSpecification
  {
    FramebufferTextureSpecification() = default;
    explicit FramebufferTextureSpecification(FramebufferTextureFormat format)
      : TextureFormat(format)
    {
    }

    FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
    // TODO: filtering/wrap
  };

  struct FramebufferAttachmentSpecification
  {
    FramebufferAttachmentSpecification() = default;
    explicit FramebufferAttachmentSpecification(
      std::initializer_list<FramebufferTextureSpecification> attachments)
      : Attachments(attachments)
    {
    }

    std::vector<FramebufferTextureSpecification> Attachments;
  };

  struct FramebufferSpecification
  {
    uint32_t                           Width = 512;
    uint32_t                           Height = 512;
    FramebufferAttachmentSpecification Attachments;
    int32_t                            Samples = 1;
    bool                               SwapChainTarget = false;
  };

  /**
   * @brief Virtual base class for a framebuffer
   *
   */
  class IFramebuffer
  {
  public:
    virtual ~IFramebuffer() = default;

    /**
     * @brief Binds the frame buffer
     *
     */
    virtual void
    Bind() = 0;

    /**
     * @brief Unbinds the frame buffer
     *
     */
    virtual void
    Unbind() = 0;

    /**
     * @brief Resizes the frame buffer to a given size
     *
     * @param width Pixel width
     * @param height Pixel height
     */
    virtual void
    Resize(uint32_t width, uint32_t height) = 0;

    /**
     * @brief Sets the index of the buffer to draw to
     *
     * @param index Color attachment index
     */
    virtual void
    SetDrawBuffer(uint32_t index) = 0;

    /**
     * @brief Updates the MSAA sample count for the frame buffer
     *
     * @param samples Amount of samples
     */
    virtual void
    SetSamples(uint32_t samples) = 0;

    /**
     * @brief Reads a pixel back from the framebuffer
     *
     * @param attachmentIndex Specifies which attachment to read from
     * @param xCoord X pixel coordinate
     * @param yCoord Y pixel coordinate
     * @return Integer value at the given pixel
     */
    virtual auto
    ReadPixel(uint32_t attachmentIndex, int xCoord, int yCoord) -> uint32_t = 0;

    /**
     * @brief Clears an attachment with a given integer value
     *
     * @param attachmentIndex Specifies the attachment to clear
     * @param value Integer value to clear with
     */
    virtual void
    ClearAttachment(uint32_t attachmentIndex, int value) = 0;

    /**
     * @brief Clears the whole frame buffer
     *
     */
    virtual void
    Clear() = 0;

    /**
     * @brief Clears a frame buffer attachment with a given color
     *
     * @param index Attachment index
     * @param clearColor 4 component color to clear the frame buffer with
     */
    virtual void
    Clear(uint32_t index, glm::vec4 clearColor) = 0;

    /**
     * @brief Gets a color attachment of the frame buffer
     *
     * @param index Index of which color attachment to get
     * @return Returns a shared pointer to the frame buffer if the index is
     * valid
     */
    [[nodiscard]] virtual auto
    GetColorAttachment(uint32_t index = 0) const
      -> std::optional<std::shared_ptr<ITexture>> = 0;

    /**
     * @brief Gets the depth attachment of the framebuffer
     *
     * @return Returns a reference to the depth attachment if present
     */
    [[nodiscard]] virtual auto
    GetDepthAttachment() const -> std::optional<std::shared_ptr<ITexture>> = 0;

    /**
     * @brief Retrieves the specification the frame buffer is using
     *
     * @return The frame buffer specification
     */
    [[nodiscard]] virtual auto
    GetSpecification() const -> const FramebufferSpecification& = 0;
  };
}