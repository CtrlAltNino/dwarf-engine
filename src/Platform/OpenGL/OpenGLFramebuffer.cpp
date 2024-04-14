#include "dpch.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Dwarf {
  static const uint32_t s_MaxFramebufferSize = 8192;

  namespace Utils {
    static glm::ivec2 ConvertToOpenGLCoords(const glm::ivec2& originalCoords,
                                            int textureHeight)
    {
      glm::ivec2 openGLCoords;
      openGLCoords.x = originalCoords.x;
      openGLCoords.y = textureHeight - originalCoords.y;
      return openGLCoords;
    }

    // @brief: Returns the OpenGL texture target for the given multisampled
    // state
    static GLenum TextureTarget(bool multisampled)
    {
      return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    // @brief: Creates textures with the given multisampled state
    static void CreateTextures(bool multisampled,
                               uint32_t* outID,
                               uint32_t count)
    {
      glCreateTextures(TextureTarget(multisampled), count, outID);
    }

    // @brief: Binds the texture with the given multisampled state
    static void BindTexture(bool multisampled, uint32_t id)
    {
      glBindTexture(TextureTarget(multisampled), id);
    }

    // @brief: Attaches a color texture to the framebuffer with the given
    // multisampled state
    static void AttachColorTexture(uint32_t id,
                                   int samples,
                                   GLenum internalFormat,
                                   GLenum format,
                                   GLenum type,
                                   uint32_t width,
                                   uint32_t height,
                                   int index)
    {
      bool multisampled = samples > 1;
      if (multisampled) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
                                samples,
                                internalFormat,
                                width,
                                height,
                                GL_FALSE);
      } else {
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     internalFormat,
                     width,
                     height,
                     0,
                     format,
                     type,
                     nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      }

      glFramebufferTexture2D(GL_FRAMEBUFFER,
                             GL_COLOR_ATTACHMENT0 + index,
                             TextureTarget(multisampled),
                             id,
                             0);
    }

    // @brief: Attaches a depth texture to the framebuffer with the given
    // multisampled state
    static void AttachDepthTexture(uint32_t id,
                                   int samples,
                                   GLenum format,
                                   GLenum attachmentType,
                                   uint32_t width,
                                   uint32_t height)
    {
      bool multisampled = samples > 1;
      if (multisampled) {
        glTexImage2DMultisample(
          GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
      } else {
        glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      }

      glFramebufferTexture2D(
        GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
    }

    // @brief: Checks if the given framebuffer texture format is a depth format
    static bool IsDepthFormat(FramebufferTextureFormat format)
    {
      switch (format) {
        using enum FramebufferTextureFormat;
        case FramebufferTextureFormat::DEPTH24STENCIL8:
          return true;
        case FramebufferTextureFormat::None:
        case FramebufferTextureFormat::RGBA8:
        case FramebufferTextureFormat::RED_INTEGER:
          return false;
      }

      return false;
    }

    // @brief: Converts a Dwarf framebuffer texture format to an OpenGL texture
    // format
    static GLenum DwarfFBTextureFormatToGL(FramebufferTextureFormat format)
    {
      switch (format) {
        using enum FramebufferTextureFormat;
        case FramebufferTextureFormat::RGBA8:
          return GL_RGBA8;
        case FramebufferTextureFormat::RED_INTEGER:
          return GL_RED_INTEGER;
        case FramebufferTextureFormat::None:
        case FramebufferTextureFormat::DEPTH24STENCIL8:
          return 0;
      }

      return 0;
    }
  }

  // @brief: Constructs an OpenGL framebuffer with the given specification
  OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
    : m_Specification(spec)
  {
    for (auto attachments : m_Specification.Attachments.Attachments) {
      if (!Utils::IsDepthFormat(attachments.TextureFormat))
        m_ColorAttachmentSpecifications.emplace_back(attachments);
      else
        m_DepthAttachmentSpecification = attachments;
    }

    Invalidate();
  }

  // @brief: Destroys the OpenGL framebuffer
  OpenGLFramebuffer::~OpenGLFramebuffer()
  {
    glDeleteFramebuffers(1, &m_RendererID);
    // glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
    // glDeleteTextures(1, &m_DepthAttachment);
  }

  // @brief: Invalidates the framebuffer
  void OpenGLFramebuffer::Invalidate()
  {
    // If the renderer ID is not 0, delete the framebuffer and its attachments
    if (m_RendererID) {
      glDeleteFramebuffers(1, &m_RendererID);
      // glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
      // glDeleteTextures(1, &m_DepthAttachment);

      m_ColorAttachments.clear();
      m_DepthAttachment = nullptr;
    }

    // Create the framebuffer
    glCreateFramebuffers(1, &m_RendererID);

    // Bind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    // Determine if the framebuffer is multisampled
    bool multisample = m_Specification.Samples > 1;

    // Create the color attachments
    /* if (m_ColorAttachmentSpecifications.size())
    {
            // Resize the color attachments vector to the size of the color
    attachment specifications
            //
    m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());

            // Create the textures
            // Utils::CreateTextures(multisample, m_ColorAttachments.data(),
    (uint32_t)m_ColorAttachments.size());

            // Iterate through the color attachments
            for (size_t i = 0; i < m_ColorAttachments.size(); i++)
            {
                    // Bind the texture
                    Utils::BindTexture(multisample, m_ColorAttachments[i]);

                    // Switch on the texture format
                    switch (m_ColorAttachmentSpecifications[i].TextureFormat)
                    {
                            using enum FramebufferTextureFormat;
                    case RGBA8:
                            // Attach the color texture
                            Utils::AttachColorTexture(m_ColorAttachments[i],
    m_Specification.Samples, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE,
    m_Specification.Width, m_Specification.Height, (int)i); break; case
    RED_INTEGER:
                            // Attach the color texture
                            Utils::AttachColorTexture(m_ColorAttachments[i],
    m_Specification.Samples, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT,
    m_Specification.Width, m_Specification.Height, (int)i); break; case None:
                    case DEPTH24STENCIL8:
                            break;
                    }
            }
    }

    // Create the depth attachment
    if (m_DepthAttachmentSpecification.TextureFormat !=
    FramebufferTextureFormat::None)
    {
            // Create the textures
            Utils::CreateTextures(multisample, &m_DepthAttachment, 1);

            // Bind the texture
            Utils::BindTexture(multisample, m_DepthAttachment);

            // Switch on the texture format
            switch (m_DepthAttachmentSpecification.TextureFormat)
            {
                    using enum FramebufferTextureFormat;
            case FramebufferTextureFormat::DEPTH24STENCIL8:
                    // Attach the depth texture
                    Utils::AttachDepthTexture(m_DepthAttachment,
    m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT,
    m_Specification.Width, m_Specification.Height); break; case
    FramebufferTextureFormat::None: case FramebufferTextureFormat::RGBA8: case
    FramebufferTextureFormat::RED_INTEGER: break;
            }
    } */

    // Check if the framebuffer is complete
    if (m_ColorAttachments.size() > 1) {
      // Multiple color-passes
      std::array<GLenum, 4> buffers = { GL_COLOR_ATTACHMENT0,
                                        GL_COLOR_ATTACHMENT1,
                                        GL_COLOR_ATTACHMENT2,
                                        GL_COLOR_ATTACHMENT3 };
      glDrawBuffers(buffers.size(), buffers.data());
    } else if (m_ColorAttachments.empty()) {
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
  void OpenGLFramebuffer::Bind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glViewport(0, 0, m_Specification.Width, m_Specification.Height);
  }

  // @brief: Unbinds the framebuffer
  void OpenGLFramebuffer::Unbind()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  // @brief: Resizes the framebuffer
  void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
  {
    if (width == 0 || height == 0 || width > s_MaxFramebufferSize ||
        height > s_MaxFramebufferSize) {
      return;
    }
    m_Specification.Width = width;
    m_Specification.Height = height;

    Invalidate();
  }

  // @brief: Reads a pixel from the framebuffer
  unsigned int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex,
                                            int x,
                                            int y)
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
  void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
  {
    const auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
    /* glClearTexImage(m_ColorAttachments[attachmentIndex], 0,
                                    Utils::DwarfFBTextureFormatToGL(spec.TextureFormat),
       GL_INT, &value); */
  }

  // @brief: Clears the framebuffer
  void OpenGLFramebuffer::Clear()
  {
    Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Unbind();
  }

  // @brief: Clears the framebuffer with the given color
  void OpenGLFramebuffer::Clear(glm::vec4 clearColor)
  {
    Bind();
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Unbind();
  }
}