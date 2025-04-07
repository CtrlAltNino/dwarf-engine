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

    /**
     * @brief Generates the framebuffer attachments for the framebuffer
     *
     */
    void
    GenerateAttachments();

  public:
    explicit OpenGLFramebuffer(std::shared_ptr<IDwarfLogger>    logger,
                               FramebufferSpecification         spec,
                               std::shared_ptr<ITextureFactory> textureFactory,
                               std::shared_ptr<IVramTracker>    vramTracker);
    ~OpenGLFramebuffer() override;

    /**
     * @brief Gets the underlying id of the framebuffer
     *
     * @return OpenGL Id of the frame buffer
     */
    [[nodiscard]] auto
    GetFramebufferRendererID() const -> uint32_t
    {
      return mRendererID;
    }

    /**
     * @brief Invalidates the framebuffer, so it will be rebuild
     *
     */
    void
    Invalidate();

    /**
     * @brief Binds the frame buffer
     *
     */
    void
    Bind() override;

    /**
     * @brief Unbinds the frame buffer
     *
     */
    void
    Unbind() override;

    /**
     * @brief Resizes the frame buffer to a given size
     *
     * @param width Pixel width
     * @param height Pixel height
     */
    void
    Resize(uint32_t width, uint32_t height) override;

    /**
     * @brief Sets the index of the buffer to draw to
     *
     * @param index Color attachment index
     */
    void
    SetDrawBuffer(uint32_t index) override;

    /**
     * @brief Updates the MSAA sample count for the frame buffer
     *
     * @param samples Amount of samples
     */
    void
    SetSamples(uint32_t samples) override;

    /**
     * @brief Reads a pixel back from the framebuffer
     *
     * @param attachmentIndex Specifies which attachment to read from
     * @param xCoord X pixel coordinate
     * @param yCoord Y pixel coordinate
     * @return Integer value at the given pixel
     */
    auto
    ReadPixel(uint32_t attachmentIndex, int x, int y) -> uint32_t override;

    /**
     * @brief Clears an attachment with a given integer value
     *
     * @param attachmentIndex Specifies the attachment to clear
     * @param value Integer value to clear with
     */
    void
    ClearAttachment(uint32_t attachmentIndex, int value) override;

    /**
     * @brief Gets a color attachment of the frame buffer
     *
     * @param index Index of which color attachment to get
     * @return Returns a reference to the color attachment if the index is valid
     */
    [[nodiscard]] auto
    GetColorAttachment(uint32_t index) const
      -> std::optional<const std::reference_wrapper<ITexture>> override;

    /**
     * @brief Gets the depth attachment of the framebuffer
     *
     * @return Returns a reference to the depth attachment if present
     */
    [[nodiscard]] auto
    GetDepthAttachment() const
      -> std::optional<const std::reference_wrapper<ITexture>> override;

    /**
     * @brief Clears the frame buffer
     *
     */
    void
    Clear() override;

    /**
     * @brief Clears the frame buffer with a given color
     *
     * @param clearColor 4 component color to clear the frame buffer with
     */
    void
    Clear(uint32_t index, glm::vec4 clearColor) override;

    /**
     * @brief Retrieves the specification the frame buffer is using
     *
     * @return The frame buffer specification
     */
    [[nodiscard]] auto
    GetSpecification() const -> const FramebufferSpecification& override
    {
      return mSpecification;
    }

    /**
     * @brief Deletes the framebuffer
     *
     */
    void
    DeleteFramebuffer();
  };
}