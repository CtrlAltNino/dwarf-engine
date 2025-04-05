#include "pch.h"

#include "Core/Base.h"
#include "FramebufferFactory.h"

#ifdef _WIN32
#include "Platform/OpenGL/OpenGLFramebuffer.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLFramebuffer.h"
#endif

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
      case None: break;
#ifdef _WIN32
      case D3D12: throw std::runtime_error("Unsupported Graphics API."); break;
      case OpenGL:
        return std::make_unique<OpenGLFramebuffer>(
          mLogger, spec, mTextureFactory, mVramTracker);
        break;
      case Vulkan: throw std::runtime_error("Unsupported Graphics API."); break;
#elif __linux__
      case D3D12: break; throw std::runtime_error("Unsupported Graphics API.");
      case OpenGL:
        return std::make_unique<OpenGLFramebuffer>(
          mLogger, spec, mTextureFactory, mVramTracker);
        break;
      case Vulkan: throw std::runtime_error("Unsupported Graphics API."); break;
#endif
    }
    return nullptr;
  }
} // namespace Dwarf