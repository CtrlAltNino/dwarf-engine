#pragma once

#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class OpenGLFramebuffer : public IFramebuffer
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;
    uint32_t                      m_RendererID = 0;
    FramebufferSpecification      m_Specification;
    std::vector<FramebufferTextureSpecification>
                                    m_ColorAttachmentSpecifications;
    FramebufferTextureSpecification m_DepthAttachmentSpecification{
      FramebufferTextureFormat::None
    };
    std::vector<std::unique_ptr<ITexture>> m_ColorAttachments;
    std::unique_ptr<ITexture>              m_DepthAttachment = 0;
    std::shared_ptr<ITextureFactory>       m_TextureFactory;
    std::shared_ptr<IVramTracker>          m_VramTracker;
    size_t                                 m_CurrentVramMemory = 0;

  public:
    explicit OpenGLFramebuffer(std::shared_ptr<IDwarfLogger>    logger,
                               const FramebufferSpecification&  spec,
                               std::shared_ptr<ITextureFactory> textureFactory,
                               std::shared_ptr<IVramTracker>    vramTracker);
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

    const std::optional<std::reference_wrapper<ITexture>>
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