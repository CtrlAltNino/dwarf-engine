#pragma once

#include "IVramTracker.h"
namespace Dwarf
{
  class VramTracker : public IVramTracker
  {
  private:
    size_t mTextureMemory = 0;
    size_t mBufferMemory = 0;
    size_t mFramebufferMemory = 0;
    size_t mShaderMemory = 0;
    size_t mComputeMemory = 0;

  public:
    void
    AddTextureMemory(size_t bytes) override;

    size_t
    AddTextureMemory(const TextureType&       type,
                     const TextureFormat&     format,
                     const TextureDataType&   dataType,
                     const TextureResolution& size,
                     int                      samples) override;

    size_t
    AddTextureMemory(std::shared_ptr<TextureContainer> texture) override;

    void
    RemoveTextureMemory(size_t bytes) override;

    void
    RemoveTextureMemory(const TextureType&       type,
                        const TextureFormat&     format,
                        const TextureDataType&   dataType,
                        const TextureResolution& size,
                        int                      samples) override;

    void
    RemoveTextureMemory(std::shared_ptr<TextureContainer> texture) override;

    size_t
    GetTextureMemory() const override;

    void
    AddBufferMemory(size_t bytes) override;

    void
    RemoveBufferMemory(size_t bytes) override;

    size_t
    GetBufferMemory() const override;

    void
    AddFramebufferMemory(size_t bytes) override;

    size_t
    AddFramebufferMemory(FramebufferSpecification specification) override;

    void
    RemoveFramebufferMemory(size_t bytes) override;

    void
    RemoveFramebufferMemory(FramebufferSpecification specification) override;

    size_t
    GetFramebufferMemory() const override;

    void
    AddShaderMemory(size_t bytes) override;

    void
    RemoveShaderMemory(size_t bytes) override;

    size_t
    GetShaderMemory() const override;

    void
    AddComputeMemory(size_t bytes) override;

    void
    RemoveComputeMemory(size_t bytes) override;

    size_t
    GetComputeMemory() const override;

    size_t
    GetTotalMemory() const override;
  };
}