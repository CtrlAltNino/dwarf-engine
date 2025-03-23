#pragma once

#include "Logging/IDwarfLogger.h"
#include <filesystem>
#include <fmt/format.h>
#include <glad/glad.h>

namespace Dwarf
{
  class OpenGLUtilities
  {
  public:
    static void
    CheckOpenGLError(const std::string&                   functionName,
                     const std::string&                   scope,
                     const std::shared_ptr<IDwarfLogger>& logger)
    {
      GLenum errorCode = 0;
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
        logger->LogError(Log(
          fmt::format("OpenGL Error ({}): {}", error, functionName), scope));
      }
    }

    static auto
    GetDefaultShaderPath() -> std::filesystem::path
    {
      return std::filesystem::path("data/engine/shaders/default/opengl");
    }

    static auto
    GetErrorShaderPath() -> std::filesystem::path
    {
      return std::filesystem::path("data/engine/shaders/error/opengl");
    }

    static auto
    GetIdShaderPath() -> std::filesystem::path
    {
      return std::filesystem::path("data/engine/shaders/id/opengl");
    }

    static auto
    GetGridShaderPath() -> std::filesystem::path
    {
      return std::filesystem::path("data/engine/shaders/grid/opengl");
    }

    static auto
    GetFullScreenQuadShaderPath() -> std::filesystem::path
    {
      return std::filesystem::path(
        "data/engine/shaders/fullscreen_quad/opengl");
    }

    static auto
    GetAgxTonemapShaderPath() -> std::filesystem::path
    {
      return std::filesystem::path("data/engine/shaders/agx_tonemap/opengl");
    }

    static auto
    GetDeviceInfo() -> std::string
    {
      std::string deviceInfo;
      deviceInfo +=
        "Vendor: " +
        std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR))) +
        "\n";
      deviceInfo +=
        "Renderer: " +
        std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))) +
        "\n";
      deviceInfo +=
        "API and Driver Version: " +
        std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))) +
        "\n";
      return deviceInfo;
    }
  };
}