#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/OpenGL/OpenGLUtilities.h"
#include "pch.h"
#include <SDL3/SDL_opengl.h>
#include <glad/glad.h>
#include <iostream>
#include <memory>
#include <utility>

namespace Dwarf
{
  void
  debugCallback(GLenum        source,
                GLenum        type,
                GLuint        id,
                GLenum        severity,
                GLsizei       length,
                const GLchar* message,
                const void*   userParam)
  {
    std::cout << "OpenGL Debug Message:" << '\n';
    std::cout << "    Source: " << source << '\n';
    std::cout << "    Type: " << type << '\n';
    std::cout << "    ID: " << id << '\n';
    std::cout << "    Severity: " << severity << '\n';
    std::cout << "    Message: " << message << '\n';
  }

  OpenGLContext::OpenGLContext(std::shared_ptr<IDwarfLogger> logger,
                               SDL_Window*                   windowHandle)
    : mLogger(std::move(logger))
    , mWindowHandle(windowHandle)
  {
    mLogger->LogDebug(Log("OpenGLContext created.", "OpenGLContext"));
  }

  OpenGLContext::~OpenGLContext()
  {
    mLogger->LogDebug(Log("OpenGLContext destroyed.", "OpenGLContext"));
    if (mContext != nullptr)
    {
      SDL_GL_DestroyContext(mContext);
    }
  }

  void
  OpenGLContext::Init()
  {
    mContext = SDL_GL_CreateContext(mWindowHandle);

    // Check if the OpenGL context was created successfully.
    if (mContext == nullptr)
    {
      std::cerr << "Failed to create OpenGL context." << '\n';
      return;
    }

    SDL_GL_MakeCurrent(mWindowHandle, mContext);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    // Enable OpenGL debug output
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugCallback, nullptr);
    glEnable(GL_MULTISAMPLE);

    GLint maxColorSamples = 0;
    GLint maxDepthSamples = 0;
    glGetIntegerv(GL_MAX_SAMPLES, &maxColorSamples);
    glGetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES, &maxDepthSamples);
    OpenGLUtilities::CheckOpenGLError("Init() End", "OpenGLContext", mLogger);
  }

  void
  OpenGLContext::SwapBuffers()
  {
    SDL_GL_SwapWindow(mWindowHandle);
  }
}
