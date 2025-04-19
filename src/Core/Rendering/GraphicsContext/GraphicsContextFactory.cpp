#include "pch.hpp"

#include "GraphicsContextFactory.hpp"
#include "Platform/OpenGL/OpenGLContext.hpp"

namespace Dwarf
{
  GraphicsContextFactory::GraphicsContextFactory(
    GraphicsApi                   api,
    std::shared_ptr<IDwarfLogger> logger)
    : mApi(api)
    , mLogger(std::move(logger))

  {
    mLogger->LogDebug(
      Log("GraphicsContextFactory created", "GraphicsContextFactory"));
  }

  GraphicsContextFactory::~GraphicsContextFactory()
  {
    mLogger->LogDebug(
      Log("GraphicsContextFactory destroyed", "GraphicsContextFactory"));
  }

  auto
  GraphicsContextFactory::Create(SDL_Window* window) const
    -> std::unique_ptr<IGraphicsContext>
  {
    mLogger->LogDebug(
      Log("Creating GraphicsContext", "GraphicsContextFactory"));

    switch (mApi)
    {
      using enum GraphicsApi;
      case None:
        mLogger->LogError(
          Log("Graphics API is not set", "GraphicsContextFactory"));
        throw std::runtime_error("Graphics API is not set");
      case OpenGL: return std::make_unique<OpenGLContext>(mLogger, window);
      case Vulkan:
        mLogger->LogError(Log("Vulkan API has not been implemented yet",
                              "GraphicsContextFactory"));
        throw std::runtime_error("Vulkan API has not been implemented yet");
      case D3D12:
#ifdef _WIN32
        mLogger->LogError(Log("Direct3D12 API has not been implemented yet",
                              "GraphicsContextFactory"));
        throw std::runtime_error("Direct3D12 API has not been implemented yet");
#elif __linux__
        mLogger->LogError(Log("Direct3D12 is only supported on Windows",
                              "GraphicsContextFactory"));
        throw std::runtime_error("Direct3D12 is only supported on Windows");
#endif
    }

    mLogger->LogError(
      Log("Failed to create GraphicsContext", "GraphicsContext"));

    return nullptr;
  }
} // namespace Dwarf