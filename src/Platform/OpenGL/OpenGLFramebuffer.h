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
    std::shared_ptr<IDwarfLogger>                mLogger;
    uint32_t                                     mRendererID = 0;
    FramebufferSpecification                     mSpecification;
    std::vector<FramebufferTextureSpecification> mColorAttachmentSpecifications;
    FramebufferTextureSpecification              mDepthAttachmentSpecification{
      FramebufferTextureFormat::None
    };
    std::vector<std::unique_ptr<ITexture>> mColorAttachments;
    std::unique_ptr<ITexture>              mDepthAttachment = 0;
    std::shared_ptr<ITextureFactory>       mTextureFactory;
    std::shared_ptr<IVramTracker>          mVramTracker;
    size_t                                 mCurrentVramMemory = 0;

  public:
    explicit OpenGLFramebuffer(std::shared_ptr<IDwarfLogger>    logger,
                               const FramebufferSpecification&  spec,
                               std::shared_ptr<ITextureFactory> textureFactory,
                               std::shared_ptr<IVramTracker>    vramTracker);
    ~OpenGLFramebuffer() override;

    uint32_t
    GetFramebufferRendererID() const
    {
      return mRendererID;
    }

    void
    Invalidate();

    void
    Bind() override;

    void
    Unbind() override;

    void
    Resize(uint32_t width, uint32_t height) override;

    void
    SetSamples(uint32_t samples) override;

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
      return mSpecification;
    }

    void
    DeleteFramebuffer();
  };
}