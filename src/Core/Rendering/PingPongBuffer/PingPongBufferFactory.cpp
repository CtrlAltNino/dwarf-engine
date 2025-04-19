#include "pch.hpp"

#include "Core/Rendering/PingPongBuffer/PingPongBuffer.hpp"
#include "PingPongBufferFactory.hpp"

namespace Dwarf
{
  PingPongBufferFactory::PingPongBufferFactory(
    std::shared_ptr<IDwarfLogger>        logger,
    std::shared_ptr<IFramebufferFactory> framebufferFactory)
    : mLogger(std::move(logger))
    , mFramebufferFactory(std::move(framebufferFactory))
  {
    mLogger->LogDebug(
      Log("PingPongBufferFactory created", "PingPongBufferFactory"));
  }

  PingPongBufferFactory::~PingPongBufferFactory()
  {
    mLogger->LogDebug(
      Log("PingPongBufferFactory destroyed", "PingPongBufferFactory"));
  }

  auto
  PingPongBufferFactory::Create(FramebufferSpecification spec)
    -> std::unique_ptr<IPingPongBuffer>
  {
    return std::make_unique<PingPongBuffer>(mFramebufferFactory, spec);
  }
}