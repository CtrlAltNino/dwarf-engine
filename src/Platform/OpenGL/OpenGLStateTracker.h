#pragma once

#include "IOpenGLStateTracker.h"
#include "Logging/IDwarfLogger.h"

#include <glad/glad.h>

namespace Dwarf
{
  struct ViewportState
  {
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t width = 0;
    uint32_t height = 0;

    auto
    operator==(const ViewportState& other) const -> bool
    {
      return (x == other.x) && (y == other.y) && (width == other.width) &&
             (height == other.height);
    }
  };

  class OpenGLStateTracker : public IOpenGLStateTracker
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    GLuint                        mCurrentShaderProgram = 0;
    bool                          mBlendMode = false;
    GLenum                        mBlendSource = 0;
    GLenum                        mBlendDestination = 0;
    bool                          mCullMode = false;
    GLenum                        mCullFace = 0;
    ViewportState                 mViewportState = { 0, 0, 0, 0 };
    glm::vec4                     mClearColor = { 0, 0, 0, 0 };
    bool                          mDepthMode = false;
    GLenum                        mDepthFunc = 0;

  public:
    OpenGLStateTracker(std::shared_ptr<IDwarfLogger> logger);
    ~OpenGLStateTracker() override;

    void
    SetShaderProgram(GLuint program) override;

    void
    BindFramebuffer(GLuint framebuffer) override;

    void
    BindVertexArray(GLuint vao) override;

    void
    BindBuffer(GLuint buffer) override;

    void
    SetBlendMode(bool enabled) override;

    void
    SetBlendFunction(GLenum source, GLenum destination) override;

    void
    SetDepthTest(bool enabled) override;

    void SetDepthFunction(GLenum) override;

    void
    SetStencilTest(bool enabled) override;

    void
    SetCullMode(bool enabled) override;

    void
    SetCullFace(GLenum face) override;

    void
    SetViewport(uint32_t x,
                uint32_t y,
                uint32_t width,
                uint32_t height) override;

    void
    SetClearColor(const glm::vec4& color) override;
  };
}