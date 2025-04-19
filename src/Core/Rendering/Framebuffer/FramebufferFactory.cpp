#include "pch.hpp"

#include "Core/Base.hpp"
#include "FramebufferFactory.hpp"
#include "Platform/OpenGL/OpenGLFramebuffer.hpp"

namespace Dwarf
{
  FramebufferFactory::FramebufferFactory(
    GraphicsApi                      api,
    std::shared_ptr<IDwarfLogger>    logger,
    std::shared_ptr<ITextureFactory> textureFactory,
    std::shared_ptr<IVramTracker>    vramTracker)
    : mApi(api)
    , mLogger(std::move(logger))
    , mTextureFactory(std::move(textureFactory))
    , mVramTracker(std::move(vramTracker))
  {
    mLogger->LogDebug(Log("FramebufferFactory created", "FramebufferFactory"));
  }

  auto
  FramebufferFactory::Create(const FramebufferSpecification& spec)
    -> std::unique_ptr<IFramebuffer>
  {
    mLogger->LogDebug(Log("Creating framebuffer", "FramebufferFactory"));

    switch (mApi)
    {
      using enum GraphicsApi;
      case None:
        mLogger->LogError(Log("Graphics API is not set", "FramebufferFactory"));
        throw std::runtime_error("Graphics API is not set");
      case OpenGL:
        return std::make_unique<OpenGLFramebuffer>(
          mLogger, spec, mTextureFactory, mVramTracker);
      case Vulkan:
        mLogger->LogError(
          Log("Vulkan API has not been implemented yet", "FramebufferFactory"));
        throw std::runtime_error("Vulkan API has not been implemented yet");
      case D3D12:
#ifdef _WIN32
        mLogger->LogError(Log("Direct3D12 API has not been implemented yet",
                              "FramebufferFactory"));
        throw std::runtime_error("Direct3D12 API has not been implemented yet");
#elif __linux__
        mLogger->LogError(
          Log("Direct3D12 is only supported on Windows", "FramebufferFactory"));
        throw std::runtime_error("Direct3D12 is only supported on Windows");
#endif
    }

    mLogger->LogError(
      Log("Failed to create Framebuffer", "FramebufferFactory"));

    return nullptr;
  }
} // namespace Dwarf