#pragma once
#include "Core/Base.h"
#include "Core/Rendering/Texture/ITexture.h"
#include "pch.h"

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

    virtual void
    Bind() = 0;
    virtual void
    Unbind() = 0;

    virtual void
    Resize(uint32_t width, uint32_t height) = 0;

    virtual void
    SetSamples(uint32_t samples) = 0;
    virtual auto
    ReadPixel(uint32_t attachmentIndex, int xCoord, int yCoord)
      -> unsigned int = 0;

    virtual void
    ClearAttachment(uint32_t attachmentIndex, int value) = 0;

    virtual void
    Clear() = 0;
    virtual void
    Clear(glm::vec4 clearColor) = 0;

    virtual auto
    GetColorAttachment(uint32_t index = 0) const
      -> const std::optional<std::reference_wrapper<ITexture>> = 0;

    virtual auto
    GetSpecification() const -> const FramebufferSpecification& = 0;
  };
}