#pragma once
#include "Core/Base.h"
#include "Core/Rendering/Texture/ITexture.h"

namespace Dwarf
{
  // TODO: Align with TextureCommon.h
  enum class FramebufferTextureFormat
  {
    None = 0,
    // Color
    RGBA8 = 1,
    RGBA16F = 2,
    RED_INTEGER = 3,
    // Depth/stencil
    DEPTH24STENCIL8 = 4,
    DEPTH = 5,
    STENCIL = 6
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
    uint32_t                           Samples = 1;
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
    ReadPixel(uint32_t attachmentIndex, int xCoord, int yCoord)
      -> unsigned int = 0;

    /**
     * @brief Clears an attachment with a given integer value
     *
     * @param attachmentIndex Specifies the attachment to clear
     * @param value Integer value to clear with
     */
    virtual void
    ClearAttachment(uint32_t attachmentIndex, int value) = 0;

    /**
     * @brief Clears the frame buffer
     *
     */
    virtual void
    Clear() = 0;

    /**
     * @brief Clears the frame buffer with a given color
     *
     * @param clearColor 4 component color to clear the frame buffer with
     */
    virtual void
    Clear(glm::vec4 clearColor) = 0;

    /**
     * @brief Gets a color attachment of the frame buffer
     *
     * @param index Index of which color attachment to get
     * @return Returns a reference to the frame buffer if the index is valid
     */
    [[nodiscard]] virtual auto
    GetColorAttachment(uint32_t index = 0) const
      -> std::optional<const std::reference_wrapper<ITexture>> = 0;

    /**
     * @brief Retrieves the specification the frame buffer is using
     *
     * @return The frame buffer specification
     */
    [[nodiscard]] virtual auto
    GetSpecification() const -> const FramebufferSpecification& = 0;
  };
}