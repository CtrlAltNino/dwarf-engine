#include "pch.h"

#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "OpenGLUtilities.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include <glad/glad.h>
#include <glm/ext/vector_int2.hpp>

namespace Dwarf
{
  static const uint32_t s_MaxFramebufferSize = 8192;

  namespace Utils
  {
    static auto
    ConvertToOpenGLCoords(const glm::ivec2& originalCoords,
                          uint32_t          textureHeight) -> glm::ivec2
    {
      glm::ivec2 openGLCoords;
      openGLCoords.x = originalCoords.x;
      openGLCoords.y = textureHeight - originalCoords.y;
      return openGLCoords;
    }

    static auto
    FramebufferTextureFormatToString(FramebufferTextureFormat format)
      -> std::string
    {
      switch (format)
      {
        using enum FramebufferTextureFormat;
        case RGBA8: return "RGBA8";
        case SRGBA8: return "SRGBA8";
        case RGBA16F: return "RGBA16F";
        case RED_INTEGER: return "RED_INTEGER";
        case DEPTH24STENCIL8: return "DEPTH24STENCIL8";
        case DEPTH: return "DEPTH";
        case STENCIL: return "STENCIL";
        case None: return "None";
      }

      return "None";
    }

    // @brief: Checks if the given framebuffer texture format is a depth format
    static auto
    IsDepthFormat(FramebufferTextureFormat format) -> bool
    {
      switch (format)
      {
        using enum FramebufferTextureFormat;
        case DEPTH:
        case DEPTH24STENCIL8: return true;
        case RGBA16F:
        case RGBA8:
        case SRGBA8:
        case RED_INTEGER:
        case STENCIL:
        case None: return false;
      }

      return false;
    }

    static auto
    GetFramebufferAttachment(FramebufferTextureFormat format) -> GLenum
    {
      switch (format)
      {
        using enum FramebufferTextureFormat;
        case FramebufferTextureFormat::RED_INTEGER:
        case FramebufferTextureFormat::RGBA8:
        case FramebufferTextureFormat::SRGBA8:
        case FramebufferTextureFormat::RGBA16F: return GL_COLOR_ATTACHMENT0;
        case FramebufferTextureFormat::DEPTH24STENCIL8:
          return GL_DEPTH_STENCIL_ATTACHMENT;
        case FramebufferTextureFormat::DEPTH: return GL_DEPTH_ATTACHMENT;
        case FramebufferTextureFormat::STENCIL: return GL_STENCIL_ATTACHMENT;
        case FramebufferTextureFormat::None: break;
      }

      return 0;
    }

    // @brief: Converts a Dwarf framebuffer texture format to an OpenGL texture
    // format
    static auto
    DwarfFBTextureFormatToGL(FramebufferTextureFormat format) -> GLenum
    {
      switch (format)
      {
        using enum FramebufferTextureFormat;
        case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
        case FramebufferTextureFormat::SRGBA8: return GL_SRGB8_ALPHA8;
        case FramebufferTextureFormat::RGBA16F: return GL_RGBA16F;
        case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
        case FramebufferTextureFormat::DEPTH24STENCIL8:
          return GL_DEPTH24_STENCIL8;
        case FramebufferTextureFormat::DEPTH: return GL_DEPTH_COMPONENT32;
        case FramebufferTextureFormat::STENCIL: return GL_STENCIL_INDEX8;
        case FramebufferTextureFormat::None: break;
      }

      return 0;
    }
  }

  // @brief: Constructs an OpenGL framebuffer with the given specification
  OpenGLFramebuffer::OpenGLFramebuffer(
    std::shared_ptr<IDwarfLogger>    logger,
    FramebufferSpecification         spec,
    std::shared_ptr<ITextureFactory> textureFactory,
    std::shared_ptr<IVramTracker>    vramTracker)
    : mLogger(std::move(logger))
    , mSpecification(std::move(spec))
    , mTextureFactory(std::move(textureFactory))
    , mVramTracker(std::move(vramTracker))
  {
    mLogger->LogDebug(Log("OpenGLFramebuffer created.", "OpenGLFramebuffer"));
    for (auto attachments : mSpecification.Attachments.Attachments)
    {
      if (!Utils::IsDepthFormat(attachments.TextureFormat))
      {
        mColorAttachmentSpecifications.emplace_back(attachments);
      }
      else
      {
        mDepthAttachmentSpecification = attachments;
      }
    }

    Invalidate();
  }

  // @brief: Destroys the OpenGL framebuffer
  OpenGLFramebuffer::~OpenGLFramebuffer()
  {
    DeleteFramebuffer();
  }

  // @brief: Invalidates the framebuffer
  void
  OpenGLFramebuffer::Invalidate()
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before OpenGLFramebuffer Invalidate", "OpenGLFramebuffer", mLogger);
    // If the renderer ID is not 0, delete the framebuffer and its attachments
    if (mRendererID != 0U)
    {
      DeleteFramebuffer();
    }

    // Create the framebuffer
    glCreateFramebuffers(1, &mRendererID);
    OpenGLUtilities::CheckOpenGLError(
      "glCreateFramebuffer", "OpenGLFramebuffer", mLogger);

    // Bind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
    OpenGLUtilities::CheckOpenGLError(
      "glBindFramebuffer", "OpenGLFramebuffer", mLogger);

    GenerateAttachments();

    // Check if the framebuffer is complete
    if (mColorAttachments.size() > 1)
    {
      // Multiple color-passes
      std::array<GLenum, 4> buffers = { GL_COLOR_ATTACHMENT0,
                                        GL_COLOR_ATTACHMENT1,
                                        GL_COLOR_ATTACHMENT2,
                                        GL_COLOR_ATTACHMENT3 };
      glDrawBuffers(buffers.size(), buffers.data());
      OpenGLUtilities::CheckOpenGLError(
        "glDrawBuffers", "OpenGLFramebuffer", mLogger);
    }
    else if (mColorAttachments.empty())
    {
      // Only depth-pass
      glDrawBuffer(GL_NONE);
    }

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      mLogger->LogError(
        Log("Framebuffer is not complete!", "OpenGLFramebuffer"));
    }
    else
    {
      mLogger->LogDebug(Log("Framebuffer is complete!", "OpenGLFramebuffer"));
      mCurrentVramMemory = mVramTracker->AddFramebufferMemory(mSpecification);
    }

    // Unbind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glBindFramebuffer Invalidate() end", "OpenGLFramebuffer", mLogger);
  }

  // @brief: Binds the framebuffer
  void
  OpenGLFramebuffer::Bind()
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before Bind()", "OpenGLFramebuffer", mLogger);
    glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
    OpenGLUtilities::CheckOpenGLError(
      "glBindFramebuffer Bind()", "OpenGLFramebuffer", mLogger);
    glViewport(0, 0, mSpecification.Width, mSpecification.Height);
    OpenGLUtilities::CheckOpenGLError(
      "glViewport", "OpenGLFramebuffer", mLogger);
  }

  // @brief: Unbinds the framebuffer
  void
  OpenGLFramebuffer::Unbind()
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before Unbind()", "OpenGLFramebuffer", mLogger);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glBindFramebuffer Unbind()", "OpenGLFramebuffer", mLogger);
  }

  // @brief: Resizes the framebuffer
  void
  OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
  {
    if (width == 0 || height == 0 || width > s_MaxFramebufferSize ||
        height > s_MaxFramebufferSize)
    {
      return;
    }
    mSpecification.Width = width;
    mSpecification.Height = height;

    glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
    GenerateAttachments();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void
  OpenGLFramebuffer::GenerateAttachments()
  {
    GLenum textarget =
      mSpecification.Samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    mColorAttachments.clear();

    // Create the color attachments
    if (mColorAttachmentSpecifications.size() != 0U)
    {
      // Iterate through the color attachments
      for (size_t i = 0; i < mColorAttachmentSpecifications.size(); i++)
      {
        std::shared_ptr<TextureContainer> textureData =
          std::make_shared<TextureContainer>();

        textureData->Type = TextureType::TEXTURE_2D;
        textureData->Samples = mSpecification.Samples;
        textureData->Size =
          glm::ivec2(mSpecification.Width, mSpecification.Height);

        switch (mColorAttachmentSpecifications[i].TextureFormat)
        {
          using enum FramebufferTextureFormat;
          case RGBA8:
          case SRGBA8: textureData->Format = TextureFormat::RGBA; break;
          case RGBA16F:
            textureData->Format = TextureFormat::RGBA;
            textureData->DataType = TextureDataType::FLOAT;
            break;
          case RED_INTEGER:
            textureData->Format = TextureFormat::RED;
            textureData->DataType = TextureDataType::UNSIGNED_INT;
            break;
          case DEPTH24STENCIL8:
            textureData->Format = TextureFormat::DEPTH_STENCIL;
            textureData->DataType = TextureDataType::UNSIGNED_INT;
            break;
          case DEPTH:
            textureData->Format = TextureFormat::DEPTH;
            textureData->DataType = TextureDataType::FLOAT;
            break;
          case STENCIL: textureData->Format = TextureFormat::STENCIL; break;
          case None: break;
        }

        mColorAttachments.push_back(
          std::move(mTextureFactory->Empty(textureData->Type,
                                           textureData->Format,
                                           textureData->DataType,
                                           textureData->Size,
                                           textureData->Samples)));

        glFramebufferTexture2D(
          GL_FRAMEBUFFER,
          Utils::GetFramebufferAttachment(
            mColorAttachmentSpecifications[i].TextureFormat) +
            i,
          textarget,
          mColorAttachments[i]->GetTextureID(),
          0);
        OpenGLUtilities::CheckOpenGLError(
          "glFramebufferTexture2D color attachment",
          "OpenGLFramebuffer",
          mLogger);
      }
    }

    // Create the depth attachment
    if (mDepthAttachmentSpecification.TextureFormat !=
        FramebufferTextureFormat::None)
    {
      mDepthAttachment = std::move(mTextureFactory->Empty(
        TextureType::TEXTURE_2D,
        TextureFormat::DEPTH,
        TextureDataType::FLOAT,
        glm::ivec2(mSpecification.Width, mSpecification.Height),
        mSpecification.Samples));

      glFramebufferTexture2D(GL_FRAMEBUFFER,
                             Utils::GetFramebufferAttachment(
                               mDepthAttachmentSpecification.TextureFormat),
                             textarget,
                             mDepthAttachment->GetTextureID(),
                             0);

      OpenGLUtilities::CheckOpenGLError(
        "glFramebufferTexture2D depth attachment",
        "OpenGLFramebuffer",
        mLogger);
    }
  }

  void
  OpenGLFramebuffer::SetSamples(uint32_t samples)
  {
    if (samples == mSpecification.Samples)
    {
      return;
    }

    GLint maxSamples = 0;
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);

    if ((samples <= 0) || (samples > maxSamples))
    {
      return;
    }

    mSpecification.Samples = samples;
    Invalidate();
  }

  // @brief: Reads a pixel from the framebuffer
  auto
  OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
    -> uint32_t
  {
    Bind();
    glm::ivec2 convertedCoords =
      Utils::ConvertToOpenGLCoords({ x, y }, mSpecification.Height);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    OpenGLUtilities::CheckOpenGLError(
      "glReadBuffer", "OpenGLFramebuffer", mLogger);

    GLuint pixel = 0;
    glReadPixels(convertedCoords.x,
                 convertedCoords.y,
                 1,
                 1,
                 GL_RED_INTEGER,
                 GL_UNSIGNED_INT,
                 &pixel);
    OpenGLUtilities::CheckOpenGLError(
      "glReadPixels", "OpenGLFramebuffer", mLogger);

    Unbind();
    return pixel;
  }

  // @brief: Clears an attachment of the framebuffer
  void
  OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
  {
    const auto& spec = mColorAttachmentSpecifications[attachmentIndex];
    glClearTexImage(mColorAttachments[attachmentIndex]->GetTextureID(),
                    0,
                    Utils::DwarfFBTextureFormatToGL(spec.TextureFormat),
                    GL_INT,
                    &value);
    OpenGLUtilities::CheckOpenGLError(
      "glClearTexImage", "OpenGLFramebuffer", mLogger);
  }

  auto
  OpenGLFramebuffer::GetColorAttachment(uint32_t index = 0) const
    -> std::optional<const std::reference_wrapper<ITexture>>
  {
    if (index < mColorAttachments.size())
    {
      // Return the address of the value at the specified index
      return *mColorAttachments[index];
    }

    // If the index is out of bounds, return nullptr or handle the error
    // accordingly
    return std::nullopt;
  }

  // @brief: Clears the framebuffer
  void
  OpenGLFramebuffer::Clear()
  {
    Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    OpenGLUtilities::CheckOpenGLError("glClear", "OpenGLFramebuffer", mLogger);
    Unbind();
  }

  // @brief: Clears the framebuffer with the given color
  void
  OpenGLFramebuffer::Clear(glm::vec4 clearColor)
  {
    Bind();
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    OpenGLUtilities::CheckOpenGLError("glClear", "OpenGLFramebuffer", mLogger);
    Unbind();
  }

  void
  OpenGLFramebuffer::DeleteFramebuffer()
  {
    mLogger->LogDebug(Log("Deleting framebuffer", "OpenGLFramebuffer"));
    glDeleteFramebuffers(1, &mRendererID);
    OpenGLUtilities::CheckOpenGLError(
      "glDeleteFramebuffers", "OpenGLFramebuffer", mLogger);
    mColorAttachments.clear();
    mDepthAttachment = nullptr;
    mVramTracker->RemoveFramebufferMemory(mCurrentVramMemory);
  }
}