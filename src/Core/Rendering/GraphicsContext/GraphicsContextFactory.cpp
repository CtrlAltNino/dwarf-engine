#include "GraphicsContextFactory.h"
#include <stdexcept>

#if _WIN32
#include "Platform/OpenGL/OpenGLContext.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLContext.h"
#elif __APPLE__
// #include "Platform/Metal/MetalContext.h"
#endif

namespace Dwarf
{
  GraphicsContextFactory::GraphicsContextFactory(
    std::shared_ptr<IDwarfLogger> logger,
    GraphicsApi                   api)
    : mLogger(logger)
    , mApi(api)
  {
    mLogger->LogDebug(
      Log("GraphicsContextFactory created.", "GraphicsContext"));
  }

  GraphicsContextFactory::~GraphicsContextFactory()
  {
    mLogger->LogDebug(
      Log("GraphicsContextFactory destroyed.", "GraphicsContext"));
  }

  std::unique_ptr<IGraphicsContext>
  GraphicsContextFactory::Create(SDL_Window* window) const
  {
    mLogger->LogDebug(Log("Creating GraphicsContext...", "GraphicsContext"));
    switch (mApi)
    {
      using enum GraphicsApi;
      case None: break;
#if _WIN32
      case D3D12:
        std::runtime_error("Vulkan API has not been implemented yet.");
        break;
      case Metal:
        std::runtime_error("Vulkan API has not been implemented yet.");
        break;
      case OpenGL:
        {
          mLogger->LogDebug(
            Log("Creating OpenGLContext...", "GraphicsContext"));
          return std::make_unique<OpenGLContext>(mLogger, window);
          break;
        }
      case Vulkan:
        std::runtime_error("Vulkan API has not been implemented yet.");
        break;
#elif __linux__
      case D3D12:
        std::runtime_error("Vulkan API has not been implemented yet.");
        break;
      case Metal:
        std::runtime_error("Vulkan API has not been implemented yet.");
        break;
      case OpenGL:
        {
          mLogger->LogDebug(
            Log("Creating OpenGLContext...", "GraphicsContext"));
          return std::make_unique<OpenGLContext>(mLogger, window);
          break;
        }
      case Vulkan:
        std::runtime_error("Vulkan API has not been implemented yet.");
        break;
#elif __APPLE__
      case D3D12: break;
      case Metal:
        // return
        // std::make_unique<MetalContext>(static_cast<SDL_Window*>(window)); -
        // NOT SUPPORTED YET
        break;
      case OpenGL: break;
      case Vulkan: break;
#endif
    }

    mLogger->LogError(
      Log("Failed to create GraphicsContext.", "GraphicsContext"));

    return nullptr;
  }
} // namespace Dwarf