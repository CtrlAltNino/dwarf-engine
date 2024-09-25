#pragma once

#include "Logging/IDwarfLogger.h"
#include <glad/glad.h>

namespace Dwarf
{
  class OpenGLUtilities
  {
  public:
    static void
    CheckOpenGLError(const std::string&            functionName,
                     std::shared_ptr<IDwarfLogger> logger)
    {
      GLenum errorCode;
      while ((errorCode = glGetError()) != GL_NO_ERROR)
      {
        std::string error;
        switch (errorCode)
        {
          case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
          case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
          case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
          case GL_STACK_OVERFLOW: error = "STACK_OVERFLOW"; break;
          case GL_STACK_UNDERFLOW: error = "STACK_UNDERFLOW"; break;
          case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
          case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }
        logger->LogError(
          Log(fmt::format("OpenGL Error ({}): {}", error, functionName),
              "OpenGLFramebuffer"));
      }
    }
  };
}