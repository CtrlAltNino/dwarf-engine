#include "FramebufferFactory.h"
#include "Core/Base.h"
#include <memory>
#include <utility>

#ifdef _WIN32
// #include "Platform/Direct3D12/D3D12Framebuffer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
// #include "Platform/Vulkan/VulkanFramebuffer.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLFramebuffer.h"
// #include "Platform/Vulkan/VulkanFramebuffer.h"
#elif __APPLE__
// #include "Platform/Metal/MetalFramebuffer.h"
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
      case D3D12:
        // return std::make_shared<D3D12Framebuffer>(spec);
        break;
      case Metal: break;
      case OpenGL:
        return std::make_unique<OpenGLFramebuffer>(
          mLogger, spec, mTextureFactory, mVramTracker);
        break;
      case Vulkan:
        // return std::make_shared<VulkanFramebuffer>(spec);
        break;
#elif __linux__
      case D3D12: break;
      case Metal: break;
      case OpenGL:
        return std::make_unique<OpenGLFramebuffer>(
          mLogger, spec, mTextureFactory, mVramTracker);
        break;
      case Vulkan: break;
#elif __APPLE__
      case D3D12: break;
      case Metal:
        // return std::make_shared<MetalFramebuffer>(spec);
        break;
      case OpenGL: break;
      case Vulkan: break;
#endif
    }
    return nullptr;
  }
} // namespace Dwarf