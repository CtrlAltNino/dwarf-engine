#include "Core/Rendering/TextureCreator.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

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
  OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
    : m_Specification(spec)
  {
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
    // If the renderer ID is not 0, delete the framebuffer and its attachments
    if (m_RendererID)
    {
      DeleteFramebuffer();
    }

    GLenum textarget =
      m_Specification.Samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    // Create the framebuffer
    glCreateFramebuffers(1, &m_RendererID);

    // Bind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    // Create the color attachments
    if (m_ColorAttachmentSpecifications.size())
    {
      // Iterate through the color attachments
      for (size_t i = 0; i < m_ColorAttachmentSpecifications.size(); i++)
      {
        m_ColorAttachments.push_back(TextureCreator::Empty(
          m_ColorAttachmentSpecifications[i],
          { m_Specification.Width, m_Specification.Height },
          m_Specification.Samples));

        glFramebufferTexture2D(
          GL_FRAMEBUFFER,
          Utils::GetFramebufferAttachment(
            m_ColorAttachmentSpecifications[i].TextureFormat) +
            i,
          textarget,
          m_ColorAttachments[i]->GetTextureID(),
          0);
      }
    }

    // Create the depth attachment
    if (m_DepthAttachmentSpecification.TextureFormat !=
        FramebufferTextureFormat::None)
    {
      m_DepthAttachment =
        TextureCreator::Empty(m_DepthAttachmentSpecification,
                              { m_Specification.Width, m_Specification.Height },
                              m_Specification.Samples);

      glFramebufferTexture2D(GL_FRAMEBUFFER,
                             Utils::GetFramebufferAttachment(
                               m_DepthAttachmentSpecification.TextureFormat),
                             textarget,
                             m_DepthAttachment->GetTextureID(),
                             0);
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
    }
    else if (m_ColorAttachments.empty())
    {
      // Only depth-pass
      glDrawBuffer(GL_NONE);
    }

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      std::cerr << "Framebuffer is not complete!" << std::endl;

    // Unbind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  // @brief: Binds the framebuffer
  void
  OpenGLFramebuffer::Bind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glViewport(0, 0, m_Specification.Width, m_Specification.Height);
  }

  // @brief: Unbinds the framebuffer
  void
  OpenGLFramebuffer::Unbind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

    GLuint pixel;
    glReadPixels(convertedCoords.x,
                 convertedCoords.y,
                 1,
                 1,
                 GL_RED_INTEGER,
                 GL_UNSIGNED_INT,
                 &pixel);

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
  }

  const std::shared_ptr<Texture>
  OpenGLFramebuffer::GetColorAttachment(uint32_t index = 0) const
  {
    if (index < m_ColorAttachments.size())
    {
      // Return the address of the value at the specified index
      return m_ColorAttachments[index];
    }
    else
    {
      // If the index is out of bounds, return nullptr or handle the error
      // accordingly
      return nullptr;
    }
  }

  // @brief: Clears the framebuffer
  void
  OpenGLFramebuffer::Clear()
  {
    Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Unbind();
  }

  // @brief: Clears the framebuffer with the given color
  void
  OpenGLFramebuffer::Clear(glm::vec4 clearColor)
  {
    Bind();
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Unbind();
  }

  void
  OpenGLFramebuffer::DeleteFramebuffer()
  {
    glDeleteFramebuffers(1, &m_RendererID);
    m_ColorAttachments.clear();
    m_DepthAttachment = nullptr;
  }
}