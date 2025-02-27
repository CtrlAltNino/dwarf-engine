#include "OpenGLStateTracker.h"
#include "IOpenGLStateTracker.h"
#include "Logging/IDwarfLogger.h"
#include "OpenGLUtilities.h"
#include <glad/glad.h>

namespace Dwarf
{
  OpenGLStateTracker::OpenGLStateTracker(std::shared_ptr<IDwarfLogger> logger)
    : m_Logger(logger)
  {
  }

  void
  OpenGLStateTracker::SetShaderProgram(GLuint program)
  {
    if (m_CurrentShaderProgram != program)
    {
      glUseProgram(program);
      m_CurrentShaderProgram = program;

      OpenGLUtilities::CheckOpenGLError(
        "glUseProgram", "OpenGLRendererApi", m_Logger);
    }
  }

  void
  OpenGLStateTracker::BindFramebuffer(GLuint framebuffer)
  {
  }

  void
  OpenGLStateTracker::BindVertexArray(GLuint vao)
  {
  }

  void
  OpenGLStateTracker::BindBuffer(GLuint buffer)
  {
  }

  void
  OpenGLStateTracker::SetBlendMode(bool enabled)
  {
    if (m_BlendMode != enabled)
    {
      if (enabled)
      {
        glEnable(GL_BLEND);
        OpenGLUtilities::CheckOpenGLError(
          "glEnable GL_BLEND", "OpenGLRendererApi", m_Logger);
      }
      else
      {
        glDisable(GL_BLEND);
        OpenGLUtilities::CheckOpenGLError(
          "glDisable GL_BLEND", "OpenGLRendererApi", m_Logger);
      }

      m_BlendMode = enabled;
    }
  }

  void
  OpenGLStateTracker::SetBlendFunction(GLenum source, GLenum destination)
  {
    if ((m_BlendSource != source) || (m_BlendDestination != destination))
    {
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      OpenGLUtilities::CheckOpenGLError(
        "glBlendFunc", "OpenGLRendererApi", m_Logger);

      m_BlendSource = source;
      m_BlendDestination = destination;
    }
  }

  void
  OpenGLStateTracker::SetDepthTest(bool enabled)
  {
    if (enabled != m_DepthMode)
    {
      if (enabled)
      {
        glEnable(GL_DEPTH_TEST);
        OpenGLUtilities::CheckOpenGLError(
          "glEnable GL_DEPTH_TEST", "OpenGLRendererApi", m_Logger);
      }
      else
      {
        glDisable(GL_DEPTH_TEST);
        OpenGLUtilities::CheckOpenGLError(
          "glDisable GL_DEPTH_TEST", "OpenGLRendererApi", m_Logger);
      }

      m_DepthMode = enabled;
    }
  }

  void
  OpenGLStateTracker::SetDepthFunction(GLenum depthFunc)
  {
    if (m_DepthFunc != depthFunc)
    {
      glDepthFunc(depthFunc);
      OpenGLUtilities::CheckOpenGLError(
        "glDepthFunc", "OpenGLRendererApi", m_Logger);

      m_DepthFunc = depthFunc;
    }
  }

  void
  OpenGLStateTracker::SetStencilTest(bool enabled)
  {
  }

  void
  OpenGLStateTracker::SetCullMode(bool enabled)
  {
    if (m_CullMode != enabled)
    {
      if (enabled)
      {
        glEnable(GL_CULL_FACE);
        OpenGLUtilities::CheckOpenGLError(
          "glEnable GL_CULL_FACE", "OpenGLRendererApi", m_Logger);
      }
      else
      {
        glDisable(GL_CULL_FACE);
        OpenGLUtilities::CheckOpenGLError(
          "glDisable GL_CULL_FACE", "OpenGLRendererApi", m_Logger);
      }

      m_CullMode = enabled;
    }
  }

  void
  OpenGLStateTracker::SetCullFace(GLenum face)
  {
    if (m_CullFace != face)
    {
      glCullFace(face);
      OpenGLUtilities::CheckOpenGLError(
        "glCullFace", "OpenGLRendererApi", m_Logger);

      m_CullFace = face;
    }
  }

  void
  OpenGLStateTracker::SetViewport(uint32_t x,
                                  uint32_t y,
                                  uint32_t width,
                                  uint32_t height)
  {
    if (m_ViewportState != ViewportState(x, y, width, height))
    {
      OpenGLUtilities::CheckOpenGLError(
        "Before setting viewport", "OpenGLRendererApi", m_Logger);
      glViewport(x, y, width, height);
      OpenGLUtilities::CheckOpenGLError(
        "glViewport", "OpenGLRendererApi", m_Logger);
      m_ViewportState = { x, y, width, height };
    }
  }

  void
  OpenGLStateTracker::SetClearColor(const glm::vec4& color)
  {
    if (m_ClearColor != color)
    {
      m_ClearColor = color;

      OpenGLUtilities::CheckOpenGLError(
        "Before setting clear color", "OpenGLRendererApi", m_Logger);
      glClearColor(color.r, color.g, color.b, color.a);
      OpenGLUtilities::CheckOpenGLError(
        "glClearColor", "OpenGLRendererApi", m_Logger);
    }
  }
}