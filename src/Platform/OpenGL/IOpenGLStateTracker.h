#pragma once

#include <glad/glad.h>

namespace Dwarf
{
  class IOpenGLStateTracker
  {
  public:
    virtual ~IOpenGLStateTracker() = default;

    virtual void
    SetShaderProgram(GLuint program) = 0;

    virtual void
    BindFramebuffer(GLuint framebuffer) = 0;

    virtual void
    BindVertexArray(GLuint vao) = 0;

    virtual void
    BindBuffer(GLuint buffer) = 0;

    virtual void
    SetBlendMode(bool enabled) = 0;

    virtual void
    SetBlendFunction(GLenum source, GLenum destination) = 0;

    virtual void
    SetDepthTest(bool enabled) = 0;

    virtual void SetDepthFunction(GLenum) = 0;

    virtual void
    SetStencilTest(bool enabled) = 0;

    virtual void
    SetCullMode(bool enabled) = 0;

    virtual void
    SetCullFace(GLenum face) = 0;

    virtual void
    SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

    virtual void
    SetClearColor(const glm::vec4& color) = 0;
  };
}