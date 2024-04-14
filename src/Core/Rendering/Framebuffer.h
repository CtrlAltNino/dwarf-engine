#pragma once

#include "Core/Base.h"
#include "Core/Rendering/Texture.h"
#include <glm/glm.hpp>

namespace Dwarf {

  enum class FramebufferTextureFormat
  {
    None = 0,

    // Color
    RGBA8 = 1,
    RED_INTEGER = 2,

    // Depth/stencil
    DEPTH24STENCIL8 = 3,

    // Defaults
    Depth = DEPTH24STENCIL8
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
    uint32_t Width = 0;
    uint32_t Height = 0;
    FramebufferAttachmentSpecification Attachments;
    uint32_t Samples = 1;

    bool SwapChainTarget = false;
  };

  class Framebuffer
  {
  public:
    virtual ~Framebuffer() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;
    virtual unsigned int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

    virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

    virtual void Clear() = 0;
    virtual void Clear(glm::vec4 clearColor) = 0;

    virtual const Ref<Texture> GetColorAttachment(uint32_t index = 0) const = 0;

    virtual const FramebufferSpecification& GetSpecification() const = 0;

    static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
  };
}