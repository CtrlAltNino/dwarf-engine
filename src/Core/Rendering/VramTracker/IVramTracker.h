#pragma once

#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include <cstddef>
namespace Dwarf
{
  class IVramTracker
  {
  public:
    virtual void
    AddTextureMemory(size_t bytes) = 0;

    virtual size_t
    AddTextureMemory(const TextureType&       type,
                     const TextureFormat&     format,
                     const TextureDataType&   dataType,
                     const TextureResolution& size,
                     int                      samples) = 0;

    virtual size_t
    AddTextureMemory(std::shared_ptr<TextureContainer> texture) = 0;

    virtual void
    RemoveTextureMemory(size_t bytes) = 0;

    virtual void
    RemoveTextureMemory(const TextureType&       type,
                        const TextureFormat&     format,
                        const TextureDataType&   dataType,
                        const TextureResolution& size,
                        int                      samples) = 0;

    virtual void
    RemoveTextureMemory(std::shared_ptr<TextureContainer> texture) = 0;

    virtual size_t
    GetTextureMemory() const = 0;

    virtual void
    AddBufferMemory(size_t bytes) = 0;

    virtual void
    RemoveBufferMemory(size_t bytes) = 0;

    virtual size_t
    GetBufferMemory() const = 0;

    virtual void
    AddFramebufferMemory(size_t bytes) = 0;

    virtual size_t
    AddFramebufferMemory(FramebufferSpecification specification) = 0;

    virtual void
    RemoveFramebufferMemory(size_t bytes) = 0;

    virtual void
    RemoveFramebufferMemory(FramebufferSpecification specification) = 0;

    virtual size_t
    GetFramebufferMemory() const = 0;

    virtual void
    AddShaderMemory(size_t bytes) = 0;

    virtual void
    RemoveShaderMemory(size_t bytes) = 0;

    virtual size_t
    GetShaderMemory() const = 0;

    virtual void
    AddComputeMemory(size_t bytes) = 0;

    virtual void
    RemoveComputeMemory(size_t bytes) = 0;

    virtual size_t
    GetComputeMemory() const = 0;

    virtual size_t
    GetTotalMemory() const = 0;
  };
}