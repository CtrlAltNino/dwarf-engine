#include "OpenGLUtilities.h"
#include "pch.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include <glad/glad.h>
#include <memory>

namespace Dwarf
{
  static const uint32_t s_MaxFramebufferSize = 8192;

  namespace Utils
  {
    static glm::ivec2
    ConvertToOpenGLCoords(const glm::ivec2& originalCoords, int textureHeight)
    {
      glm::ivec2 openGLCoords;
      openGLCoords.x = originalCoords.x;
      openGLCoords.y = textureHeight - originalCoords.y;
      return openGLCoords;
    }

    static std::string
    FramebufferTextureFormatToString(FramebufferTextureFormat format)
    {
      switch (format)
      {
        using enum FramebufferTextureFormat;
        case RGBA8: return "RGBA8";
        case RED_INTEGER: return "RED_INTEGER";
        case DEPTH24STENCIL8: return "DEPTH24STENCIL8";
        case DEPTH: return "DEPTH";
        case STENCIL: return "STENCIL";
        case None: return "None";
      }

      return "None";
    }

    // @brief: Checks if the given framebuffer texture format is a depth format
    static bool
    IsDepthFormat(FramebufferTextureFormat format)
    {
      switch (format)
      {
        using enum FramebufferTextureFormat;
        case DEPTH: return true;
        case DEPTH24STENCIL8: return true;
        case RGBA8: return false;
        case RED_INTEGER: return false;
        case STENCIL: return false;
        case None: return false;
      }

      return false;
    }

    static GLenum
    GetFramebufferAttachment(FramebufferTextureFormat format)
    {
      switch (format)
      {
        using enum FramebufferTextureFormat;
        case FramebufferTextureFormat::RED_INTEGER: return GL_COLOR_ATTACHMENT0;
        case FramebufferTextureFormat::RGBA8: return GL_COLOR_ATTACHMENT0;
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
    static GLenum
    DwarfFBTextureFormatToGL(FramebufferTextureFormat format)
    {
      switch (format)
      {
        using enum FramebufferTextureFormat;
        case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
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
    const FramebufferSpecification&  spec,
    std::shared_ptr<ITextureFactory> textureFactory)
    : m_Logger(logger)
    , m_Specification(spec)
    , m_TextureFactory(textureFactory)
  {
    m_Logger->LogInfo(Log("OpenGLFramebuffer created.", "OpenGLFramebuffer"));
    for (auto attachments : m_Specification.Attachments.Attachments)
    {
      if (!Utils::IsDepthFormat(attachments.TextureFormat))
      {
        m_ColorAttachmentSpecifications.emplace_back(attachments);
      }
      else
      {
        m_DepthAttachmentSpecification = attachments;
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
      "Before OpenGLFramebuffer Invalidate", "OpenGLFramebuffer", m_Logger);
    // If the renderer ID is not 0, delete the framebuffer and its attachments
    if (m_RendererID)
    {
      DeleteFramebuffer();
    }

    GLenum textarget =
      m_Specification.Samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    // Create the framebuffer
    glCreateFramebuffers(1, &m_RendererID);
    OpenGLUtilities::CheckOpenGLError(
      "glCreateFramebuffer", "OpenGLFramebuffer", m_Logger);

    // Bind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    OpenGLUtilities::CheckOpenGLError(
      "glBindFramebuffer", "OpenGLFramebuffer", m_Logger);

    // Create the color attachments
    if (m_ColorAttachmentSpecifications.size())
    {
      // Iterate through the color attachments
      for (size_t i = 0; i < m_ColorAttachmentSpecifications.size(); i++)
      {
        // TextureType       Type = TextureType::TEXTURE_2D;
        // TextureFormat     Format = TextureFormat::RGBA;
        // TextureDataType   DataType = TextureDataType::UNSIGNED_BYTE;
        // TextureResolution Size = glm::ivec2(0);
        // void*             ImageData = nullptr;
        // int               Samples = 1;
        std::shared_ptr<TextureContainer> textureData =
          std::make_shared<TextureContainer>();

        textureData->Type = TextureType::TEXTURE_2D;
        textureData->Samples = m_Specification.Samples;
        textureData->Size =
          glm::ivec2(m_Specification.Width, m_Specification.Height);

        switch (m_ColorAttachmentSpecifications[i].TextureFormat)
        {
          using enum FramebufferTextureFormat;
          case RGBA8: textureData->Format = TextureFormat::RGBA; break;
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

        // m_ColorAttachments.push_back(m_Tex tureFactory->CreateEmptyTexture(
        //   m_ColorAttachmentSpecifications[i],
        //   { m_Specification.Width, m_Specification.Height },
        //   m_Specification.Samples));

        m_ColorAttachments.push_back(std::move(m_TextureFactory->Empty(
          TextureType::TEXTURE_2D,
          TextureFormat::RGBA,
          TextureDataType::UNSIGNED_BYTE,
          glm::ivec2(m_Specification.Width, m_Specification.Height),
          m_Specification.Samples)));

        glFramebufferTexture2D(
          GL_FRAMEBUFFER,
          Utils::GetFramebufferAttachment(
            m_ColorAttachmentSpecifications[i].TextureFormat) +
            i,
          textarget,
          m_ColorAttachments[i]->GetTextureID(),
          0);
        OpenGLUtilities::CheckOpenGLError(
          "glFramebufferTexture2D color attachment",
          "OpenGLFramebuffer",
          m_Logger);
      }
    }

    // Create the depth attachment
    if (m_DepthAttachmentSpecification.TextureFormat !=
        FramebufferTextureFormat::None)
    {
      // switch (parameters.TextureFormat)
      // {
      //   using enum FramebufferTextureFormat;
      //   case RGBA8: textureData->Format = TextureFormat::RGBA; break;
      //   case RED_INTEGER:
      //     textureData->Format = TextureFormat::RED;
      //     textureData->DataType = TextureDataType::UNSIGNED_INT;
      //     break;
      //   case DEPTH24STENCIL8:
      //     textureData->Format = TextureFormat::DEPTH_STENCIL;
      //     break;
      //   case DEPTH:
      //     textureData->Format = TextureFormat::DEPTH;
      //     textureData->DataType = TextureDataType::FLOAT;
      //     break;
      //   case STENCIL: textureData->Format = TextureFormat::STENCIL; break;
      //   case None: break;
      // }
      // m_DepthAttachment =
      //   TextureCreator::Empty(m_DepthAttachmentSpecification,
      //                         { m_Specification.Width, m_Specification.Height
      //                         }, m_Specification.Samples);

      m_DepthAttachment = std::move(m_TextureFactory->Empty(
        TextureType::TEXTURE_2D,
        TextureFormat::DEPTH,
        TextureDataType::FLOAT,
        glm::ivec2(m_Specification.Width, m_Specification.Height),
        m_Specification.Samples));

      glFramebufferTexture2D(GL_FRAMEBUFFER,
                             Utils::GetFramebufferAttachment(
                               m_DepthAttachmentSpecification.TextureFormat),
                             textarget,
                             m_DepthAttachment->GetTextureID(),
                             0);

      OpenGLUtilities::CheckOpenGLError(
        "glFramebufferTexture2D depth attachment",
        "OpenGLFramebuffer",
        m_Logger);
    }

    // Check if the framebuffer is complete
    if (m_ColorAttachments.size() > 1)
    {
      // Multiple color-passes
      std::array<GLenum, 4> buffers = { GL_COLOR_ATTACHMENT0,
                                        GL_COLOR_ATTACHMENT1,
                                        GL_COLOR_ATTACHMENT2,
                                        GL_COLOR_ATTACHMENT3 };
      glDrawBuffers(buffers.size(), buffers.data());
      OpenGLUtilities::CheckOpenGLError(
        "glDrawBuffers", "OpenGLFramebuffer", m_Logger);
    }
    else if (m_ColorAttachments.empty())
    {
      // Only depth-pass
      glDrawBuffer(GL_NONE);
    }

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      std::cerr << "Framebuffer is not complete!" << std::endl;
    }
    else
    {
      std::cout << "Framebuffer is complete!" << std::endl;
    }

    std::cout << "Framebuffer ID: " << m_RendererID << std::endl;

    // Unbind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glBindFramebuffer Invalidate() end", "OpenGLFramebuffer", m_Logger);
  }

  // @brief: Binds the framebuffer
  void
  OpenGLFramebuffer::Bind()
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before Bind()", "OpenGLFramebuffer", m_Logger);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    OpenGLUtilities::CheckOpenGLError(
      "glBindFramebuffer Bind()", "OpenGLFramebuffer", m_Logger);
    glViewport(0, 0, m_Specification.Width, m_Specification.Height);
    OpenGLUtilities::CheckOpenGLError(
      "glViewport", "OpenGLFramebuffer", m_Logger);
  }

  // @brief: Unbinds the framebuffer
  void
  OpenGLFramebuffer::Unbind()
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before Unbind()", "OpenGLFramebuffer", m_Logger);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glBindFramebuffer Unbind()", "OpenGLFramebuffer", m_Logger);
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
    m_Specification.Width = width;
    m_Specification.Height = height;

    Invalidate();
  }

  // @brief: Reads a pixel from the framebuffer
  unsigned int
  OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
  {
    Bind();
    glm::ivec2 convertedCoords =
      Utils::ConvertToOpenGLCoords({ x, y }, m_Specification.Height);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    OpenGLUtilities::CheckOpenGLError(
      "glReadBuffer", "OpenGLFramebuffer", m_Logger);

    GLuint pixel;
    glReadPixels(convertedCoords.x,
                 convertedCoords.y,
                 1,
                 1,
                 GL_RED_INTEGER,
                 GL_UNSIGNED_INT,
                 &pixel);
    OpenGLUtilities::CheckOpenGLError(
      "glReadPixels", "OpenGLFramebuffer", m_Logger);

    Unbind();
    return pixel;
  }

  // @brief: Clears an attachment of the framebuffer
  void
  OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
  {
    const auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
    glClearTexImage(m_ColorAttachments[attachmentIndex]->GetTextureID(),
                    0,
                    Utils::DwarfFBTextureFormatToGL(spec.TextureFormat),
                    GL_INT,
                    &value);
    OpenGLUtilities::CheckOpenGLError(
      "glClearTexImage", "OpenGLFramebuffer", m_Logger);
  }

  const std::optional<std::reference_wrapper<ITexture>>
  OpenGLFramebuffer::GetColorAttachment(uint32_t index = 0) const
  {
    if (index < m_ColorAttachments.size())
    {
      // Return the address of the value at the specified index
      return *m_ColorAttachments[index];
    }
    else
    {
      // If the index is out of bounds, return nullptr or handle the error
      // accordingly
      return std::nullopt;
    }
  }

  // @brief: Clears the framebuffer
  void
  OpenGLFramebuffer::Clear()
  {
    Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    OpenGLUtilities::CheckOpenGLError("glClear", "OpenGLFramebuffer", m_Logger);
    Unbind();
  }

  // @brief: Clears the framebuffer with the given color
  void
  OpenGLFramebuffer::Clear(glm::vec4 clearColor)
  {
    Bind();
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    OpenGLUtilities::CheckOpenGLError("glClear", "OpenGLFramebuffer", m_Logger);
    Unbind();
  }

  void
  OpenGLFramebuffer::DeleteFramebuffer()
  {
    m_Logger->LogInfo(Log("Deleting framebuffer", "OpenGLFramebuffer"));
    glDeleteFramebuffers(1, &m_RendererID);
    OpenGLUtilities::CheckOpenGLError(
      "glDeleteFramebuffers", "OpenGLFramebuffer", m_Logger);
    m_ColorAttachments.clear();
    m_DepthAttachment = nullptr;
  }
}