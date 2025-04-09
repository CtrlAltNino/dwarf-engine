#include "pch.h"

#include "Logging/IDwarfLogger.h"
#include "OpenGLStateTracker.h"
#include "OpenGLUtilities.h"
#include <glad/glad.h>

namespace Dwarf
{
  OpenGLStateTracker::OpenGLStateTracker(std::shared_ptr<IDwarfLogger> logger)
    : mLogger(std::move(logger))
  {
    mLogger->LogDebug(Log("OpenGLStateTracker created", "OpenGLStateTracker"));
  }

  OpenGLStateTracker::~OpenGLStateTracker()
  {
    mLogger->LogDebug(Log("OpenGLStateTracker created", "OpenGLStateTracker"));
  }

  void
  OpenGLStateTracker::SetShaderProgram(OpenGLShader& program)
  {
    if (mCurrentShaderProgram != program.GetID())
    {
      glUseProgram(program.GetID());
      mCurrentShaderProgram = program.GetID();

      OpenGLUtilities::CheckOpenGLError(
        "glUseProgram", "OpenGLRendererApi", mLogger);
      program.ResetUniformBindings();
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
    if (mBlendMode != enabled)
    {
      if (enabled)
      {
        glEnable(GL_BLEND);
        OpenGLUtilities::CheckOpenGLError(
          "glEnable GL_BLEND", "OpenGLRendererApi", mLogger);
      }
      else
      {
        glDisable(GL_BLEND);
        OpenGLUtilities::CheckOpenGLError(
          "glDisable GL_BLEND", "OpenGLRendererApi", mLogger);
      }

      mBlendMode = enabled;
    }
  }

  void
  OpenGLStateTracker::SetBlendFunction(GLenum source, GLenum destination)
  {
    if ((mBlendSource != source) || (mBlendDestination != destination))
    {
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      OpenGLUtilities::CheckOpenGLError(
        "glBlendFunc", "OpenGLRendererApi", mLogger);

      mBlendSource = source;
      mBlendDestination = destination;
    }
  }

  void
  OpenGLStateTracker::SetDepthTest(bool enabled)
  {
    if (enabled != mDepthMode)
    {
      if (enabled)
      {
        glEnable(GL_DEPTH_TEST);
        OpenGLUtilities::CheckOpenGLError(
          "glEnable GL_DEPTH_TEST", "OpenGLRendererApi", mLogger);
      }
      else
      {
        glDisable(GL_DEPTH_TEST);
        OpenGLUtilities::CheckOpenGLError(
          "glDisable GL_DEPTH_TEST", "OpenGLRendererApi", mLogger);
      }

      mDepthMode = enabled;
    }
  }

  void
  OpenGLStateTracker::SetDepthFunction(GLenum depthFunc)
  {
    if (mDepthFunc != depthFunc)
    {
      glDepthFunc(depthFunc);
      OpenGLUtilities::CheckOpenGLError(
        "glDepthFunc", "OpenGLRendererApi", mLogger);

      mDepthFunc = depthFunc;
    }
  }

  void
  OpenGLStateTracker::SetStencilTest(bool enabled)
  {
  }

  void
  OpenGLStateTracker::SetCullMode(bool enabled)
  {
    if (mCullMode != enabled)
    {
      if (enabled)
      {
        glEnable(GL_CULL_FACE);
        OpenGLUtilities::CheckOpenGLError(
          "glEnable GL_CULL_FACE", "OpenGLRendererApi", mLogger);
      }
      else
      {
        glDisable(GL_CULL_FACE);
        OpenGLUtilities::CheckOpenGLError(
          "glDisable GL_CULL_FACE", "OpenGLRendererApi", mLogger);
      }

      mCullMode = enabled;
    }
  }

  void
  OpenGLStateTracker::SetCullFace(GLenum face)
  {
    if (mCullFace != face)
    {
      glCullFace(face);
      OpenGLUtilities::CheckOpenGLError(
        "glCullFace", "OpenGLRendererApi", mLogger);

      mCullFace = face;
    }
  }

  void
  OpenGLStateTracker::SetViewport(uint32_t x,
                                  uint32_t y,
                                  uint32_t width,
                                  uint32_t height)
  {
    if (mViewportState != ViewportState(x, y, width, height))
    {
      OpenGLUtilities::CheckOpenGLError(
        "Before setting viewport", "OpenGLRendererApi", mLogger);
      glViewport(x, y, width, height);
      OpenGLUtilities::CheckOpenGLError(
        "glViewport", "OpenGLRendererApi", mLogger);
      mViewportState = { .x = x, .y = y, .width = width, .height = height };
    }
  }

  void
  OpenGLStateTracker::SetClearColor(const glm::vec4& color)
  {
    if (mClearColor != color)
    {
      mClearColor = color;

      OpenGLUtilities::CheckOpenGLError(
        "Before setting clear color", "OpenGLRendererApi", mLogger);
      glClearColor(color.r, color.g, color.b, color.a);
      OpenGLUtilities::CheckOpenGLError(
        "glClearColor", "OpenGLRendererApi", mLogger);
    }
  }
}