#pragma once
#include "pch.h"
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
    RED_INTEGER = 2,
    // Depth/stencil
    DEPTH24STENCIL8 = 3,
    DEPTH = 4,
    STENCIL = 5
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
    virtual unsigned int
    ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

    virtual void
    ClearAttachment(uint32_t attachmentIndex, int value) = 0;

    virtual void
    Clear() = 0;
    virtual void
    Clear(glm::vec4 clearColor) = 0;

    virtual const std::optional<std::reference_wrapper<ITexture>>
    GetColorAttachment(uint32_t index = 0) const = 0;

    virtual const FramebufferSpecification&
    GetSpecification() const = 0;
  };
}