#pragma once

#include "Core/Rendering/Framebuffer.h"
#include <cstdint>

namespace Dwarf
{
  class OpenGLFramebuffer : public Framebuffer
  {
  private:
    uint32_t                 m_RendererID = 0;
    FramebufferSpecification m_Specification;
    std::vector<FramebufferTextureSpecification>
                                    m_ColorAttachmentSpecifications;
    FramebufferTextureSpecification m_DepthAttachmentSpecification{
      FramebufferTextureFormat::None
    };
    std::vector<std::shared_ptr<Texture>> m_ColorAttachments;
    std::shared_ptr<Texture>              m_DepthAttachment = 0;

  public:
    explicit OpenGLFramebuffer(const FramebufferSpecification& spec);
    ~OpenGLFramebuffer() override;

    uint32_t
    GetFramebufferRendererID() const
    {
      return m_RendererID;
    }

    void
    Invalidate();

    void
    Bind() override;

    void
    Unbind() override;

    void
    Resize(uint32_t width, uint32_t height) override;

    unsigned int
    ReadPixel(uint32_t attachmentIndex, int x, int y) override;

    void
    ClearAttachment(uint32_t attachmentIndex, int value) override;

    const std::shared_ptr<Texture>
    GetColorAttachment(uint32_t index) const override;

    void
    Clear() override;

    void
    Clear(glm::vec4 clearColor) override;

    const FramebufferSpecification&
    GetSpecification() const override
    {
      return m_Specification;
    }

    void
    DeleteFramebuffer();
  };
}