#include "pch.h"

#include "GraphicsContextFactory.h"

#if _WIN32
#include "Platform/OpenGL/OpenGLContext.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLContext.h"
#endif

namespace Dwarf
{
  GraphicsContextFactory::GraphicsContextFactory(
    GraphicsApi                   api,
    std::shared_ptr<IDwarfLogger> logger)
    : mApi(api)
    , mLogger(std::move(logger))

  {
    mLogger->LogDebug(
      Log("GraphicsContextFactory created.", "GraphicsContext"));
  }

  GraphicsContextFactory::~GraphicsContextFactory()
  {
    mLogger->LogDebug(
      Log("GraphicsContextFactory destroyed.", "GraphicsContext"));
  }

  auto
  GraphicsContextFactory::Create(SDL_Window* window) const
    -> std::unique_ptr<IGraphicsContext>
  {
    mLogger->LogDebug(Log("Creating GraphicsContext...", "GraphicsContext"));
    switch (mApi)
    {
      using enum GraphicsApi;
      case None: throw std::runtime_error("No Graphics API selected."); break;
#if _WIN32
      case D3D12:
        throw std::runtime_error(
          "Direct3D12 API has not been implemented yet.");
        break;
      case OpenGL:
        {
          mLogger->LogDebug(
            Log("Creating OpenGLContext...", "GraphicsContext"));
          return std::make_unique<OpenGLContext>(mLogger, window);
          break;
        }
      case Vulkan:
        throw std::runtime_error("Vulkan API has not been implemented yet.");
        break;
#elif __linux__
      case D3D12:
        throw throw std::runtime_error("Unsupported Graphics API selected.");
        break;
      case OpenGL:
        {
          mLogger->LogDebug(
            Log("Creating OpenGLContext...", "GraphicsContext"));
          return std::make_unique<OpenGLContext>(mLogger, window);
          break;
        }
      case Vulkan:
        throw std::runtime_error("Vulkan API has not been implemented yet.");
        break;
#endif
    }

    mLogger->LogError(
      Log("Failed to create GraphicsContext.", "GraphicsContext"));

    return nullptr;
  }
} // namespace Dwarf