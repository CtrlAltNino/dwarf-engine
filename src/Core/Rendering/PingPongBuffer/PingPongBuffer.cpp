#include "pch.hpp"

#include "PingPongBuffer.hpp"

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
  PingPongBuffer::GetReadFramebuffer() -> std::weak_ptr<IFramebuffer>
  {
    return mBuffers[static_cast<int>(!mHorizontal)];
  }

  auto
  PingPongBuffer::GetWriteFramebuffer() -> std::weak_ptr<IFramebuffer>
  {
    return mBuffers[static_cast<int>(mHorizontal)];
  }

  auto
  PingPongBuffer::GetReadTexture() const -> std::shared_ptr<ITexture>
  {
    return *mBuffers[static_cast<int>(!mHorizontal)]->GetColorAttachment(0);
  }

  auto
  PingPongBuffer::GetWriteTexture() const -> std::shared_ptr<ITexture>
  {
    return *mBuffers[static_cast<int>(mHorizontal)]->GetColorAttachment(0);
  }
}