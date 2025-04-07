#include "pch.h"

#include "PingPongBuffer.h"

namespace Dwarf
{
  PingPongBuffer::PingPongBuffer(
    std::shared_ptr<IFramebufferFactory> framebufferFactory,
    const FramebufferSpecification&      spec)
    : mFramebufferFactory(std::move(framebufferFactory))
    , mSpec(spec)
  {
    mBuffers[0] = mFramebufferFactory->Create(spec);
    mBuffers[1] = mFramebufferFactory->Create(spec);
  }

  void
  PingPongBuffer::Resize(glm::ivec2 resolution)
  {
    mBuffers[0]->Resize(resolution.x, resolution.y);
    mBuffers[1]->Resize(resolution.x, resolution.y);
  }

  void
  PingPongBuffer::Swap()
  {
    mHorizontal = !mHorizontal;
  }

  auto
  PingPongBuffer::GetReadFramebuffer() -> IFramebuffer&
  {
    return *mBuffers[static_cast<int>(!mHorizontal)];
  }

  auto
  PingPongBuffer::GetWriteFramebuffer() -> IFramebuffer&
  {
    return *mBuffers[static_cast<int>(mHorizontal)];
  }

  auto
  PingPongBuffer::GetReadTexture() const -> ITexture&
  {
    return *mBuffers[static_cast<int>(!mHorizontal)]->GetColorAttachment(0);
  }

  auto
  PingPongBuffer::GetWriteTexture() const -> ITexture&
  {
    return *mBuffers[static_cast<int>(mHorizontal)]->GetColorAttachment(0);
  }
}