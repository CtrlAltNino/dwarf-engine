#include "Platform/OpenGL/OpenGLUtilities.h"
#include "pch.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include <glad/glad.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>

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
    std::cout << "OpenGL Debug Message:" << std::endl;
    std::cout << "    Source: " << source << std::endl;
    std::cout << "    Type: " << type << std::endl;
    std::cout << "    ID: " << id << std::endl;
    std::cout << "    Severity: " << severity << std::endl;
    std::cout << "    Message: " << message << std::endl;
  }

  OpenGLContext::OpenGLContext(std::shared_ptr<IDwarfLogger> logger,
                               SDL_Window*                   windowHandle)
    : m_Logger(logger)
    , m_WindowHandle(windowHandle)
  {
    m_Logger->LogDebug(Log("OpenGLContext created.", "OpenGLContext"));
  }

  OpenGLContext::~OpenGLContext()
  {
    m_Logger->LogDebug(Log("OpenGLContext destroyed.", "OpenGLContext"));
    if (m_Context) SDL_GL_DeleteContext(m_Context);
  }

  void
  OpenGLContext::Init()
  {
    m_Context = SDL_GL_CreateContext(m_WindowHandle);

    // Check if the OpenGL context was created successfully.
    if (!m_Context)
    {
      std::cerr << "Failed to create OpenGL context." << std::endl;
      return;
    }

    SDL_GL_MakeCurrent(m_WindowHandle, m_Context);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    // Enable OpenGL debug output
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugCallback, nullptr);
    glEnable(GL_MULTISAMPLE);

    GLint maxColorSamples, maxDepthSamples;
    glGetIntegerv(GL_MAX_SAMPLES, &maxColorSamples);
    glGetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES, &maxDepthSamples);
    OpenGLUtilities::CheckOpenGLError("Init() End", "OpenGLContext", m_Logger);
  }

  void
  OpenGLContext::SwapBuffers()
  {
    SDL_GL_SwapWindow(m_WindowHandle);
  }
}
