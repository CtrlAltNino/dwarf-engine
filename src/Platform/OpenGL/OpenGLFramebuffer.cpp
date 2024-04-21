#include "Core/Rendering/TextureCreator.h"
#include "dpch.h"
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

    // @brief: Returns the OpenGL texture target for the given multisampled
    // state
    /*static GLenum
    TextureTarget(bool multisampled)
    {
      return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    // @brief: Creates textures with the given multisampled state
    static void
    CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
    {
      glCreateTextures(TextureTarget(multisampled), count, outID);
    }

    // @brief: Binds the texture with the given multisampled state
    static void
    BindTexture(bool multisampled, uint32_t id)
    {
      glBindTexture(TextureTarget(multisampled), id);
    }*/

    // @brief: Attaches a color texture to the framebuffer with the given
    // multisampled state
    // static void
    // AttachColorTexture(uint32_t id,
    //                    int      samples,
    //                    GLenum   internalFormat,
    //                    GLenum   format,
    //                    GLenum   type,
    //                    uint32_t width,
    //                    uint32_t height,
    //                    int      index)
    // {
    //   bool multisampled = samples > 1;
    //   if (multisampled)
    //   {
    //     glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
    //                             samples,
    //                             internalFormat,
    //                             width,
    //                             height,
    //                             GL_FALSE);
    //   }
    //   else
    //   {
    //     glTexImage2D(GL_TEXTURE_2D,
    //                  0,
    //                  internalFormat,
    //                  width,
    //                  height,
    //                  0,
    //                  format,
    //                  type,
    //                  nullptr);

    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //   }

    //   glFramebufferTexture2D(GL_FRAMEBUFFER,
    //                          GL_COLOR_ATTACHMENT0 + index,
    //                          TextureTarget(multisampled),
    //                          id,
    //                          0);
    // }

    // // @brief: Attaches a depth texture to the framebuffer with the given
    // // multisampled state
    // static void
    // AttachDepthTexture(uint32_t id,
    //                    int      samples,
    //                    GLenum   format,
    //                    GLenum   attachmentType,
    //                    uint32_t width,
    //                    uint32_t height)
    // {
    //   bool multisampled = samples > 1;
    //   if (multisampled)
    //   {
    //     glTexImage2DMultisample(
    //       GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height,
    //       GL_FALSE);
    //   }
    //   else
    //   {
    //     glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //   }

    //   glFramebufferTexture2D(
    //     GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
    // }

    // @brief: Checks if the given framebuffer texture format is a depth format
    static bool
    IsDepthFormat(FramebufferTextureFormat format)
    {
      switch (format)
      {
        using enum FramebufferTextureFormat;
        case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
        case FramebufferTextureFormat::None:
        case FramebufferTextureFormat::RGBA8:
        case FramebufferTextureFormat::RED_INTEGER: return false;
      }

      return false;
    }

    // @brief: Converts a Dwarf framebuffer texture format to an OpenGL texture
    // format
    /*static GLenum
    DwarfFBTextureFormatToGL(FramebufferTextureFormat format)
    {
      switch (format)
      {
        using enum FramebufferTextureFormat;
        case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
        case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
        case FramebufferTextureFormat::None:
        case FramebufferTextureFormat::DEPTH24STENCIL8: return 0;
      }

      return 0;
    }*/
  }

  // @brief: Constructs an OpenGL framebuffer with the given specification
  OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
    : m_Specification(spec)
  {
    for (auto attachments : m_Specification.Attachments.Attachments)
    {
      if (!Utils::IsDepthFormat(attachments.TextureFormat))
        m_ColorAttachmentSpecifications.emplace_back(attachments);
      else
        m_DepthAttachmentSpecification = attachments;
    }

    std::cout << "Color attachment size: "
              << m_ColorAttachmentSpecifications.size() << std::endl;

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

    std::cout << "Invalidate" << std::endl;

    // Create the framebuffer
    glCreateFramebuffers(1, &m_RendererID);

    // Bind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    // Determine if the framebuffer is multisampled
    bool multisample = m_Specification.Samples > 1;

    std::cout << "Color attachment specification size: "
              << m_ColorAttachmentSpecifications.size() << std::endl;

    // Create the color attachments
    if (m_ColorAttachmentSpecifications.size())
    {
      // Iterate through the color attachments
      for (size_t i = 0; i < m_ColorAttachmentSpecifications.size(); i++)
      {
        m_ColorAttachments.push_back(TextureCreator::Empty(
          m_ColorAttachmentSpecifications[i],
          { m_Specification.Width, m_Specification.Height }));

        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0 + i,
                               GL_TEXTURE_2D,
                               m_ColorAttachments[i]->GetTextureID(),
                               0);
      }
    }

    // Create the depth attachment
    if (m_DepthAttachmentSpecification.TextureFormat !=
        FramebufferTextureFormat::None)
    {
      m_DepthAttachment = TextureCreator::Empty(
        m_DepthAttachmentSpecification,
        { m_Specification.Width, m_Specification.Height });

      glFramebufferTexture2D(GL_FRAMEBUFFER,
                             GL_DEPTH_STENCIL_ATTACHMENT,
                             GL_TEXTURE_2D,
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
    /* glClearTexImage(m_ColorAttachments[attachmentIndex], 0,
                                    Utils::DwarfFBTextureFormatToGL(spec.TextureFormat),
       GL_INT, &value); */
  }

  const Ref<Texture>
  OpenGLFramebuffer::GetColorAttachment(uint32_t index = 0) const
  {
    std::cout << "Index: " << index << std::endl;
    std::cout << "Size: " << m_ColorAttachments.size() << std::endl;
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
    std::cout << "DeleteFramebuffer" << std::endl;
    glDeleteFramebuffers(1, &m_RendererID);
    m_ColorAttachments.clear();
    m_DepthAttachment = nullptr;
  }
}