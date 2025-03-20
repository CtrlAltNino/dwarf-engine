#include "VramTracker.h"
#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Utilities/ImageUtilities/TextureCommon.h"

namespace Dwarf
{
  struct TextureResolutionVisitor
  {
    size_t& mResolutionMultiplied;

    void
    operator()(const glm::ivec1& parameter)
    {
      mResolutionMultiplied = parameter.x;
    }

    void
    operator()(const glm::ivec2& parameter)
    {
      mResolutionMultiplied = parameter.x * parameter.y;
    }

    void
    operator()(const glm::ivec3& parameter)
    {
      mResolutionMultiplied = parameter.x * parameter.y * parameter.z;
    }
  };

  size_t
  CalculateTextureMemory(const TextureType&       type,
                         const TextureFormat&     format,
                         const TextureDataType&   dataType,
                         const TextureResolution& size,
                         int                      samples)
  {
    size_t resolutionMultiplied = 0;
    size_t numberOfComponents = 0;
    size_t bytesPerComponent = 0;

    switch (format)
    {
      case TextureFormat::RED:
      case TextureFormat::DEPTH:
      case TextureFormat::STENCIL: numberOfComponents = 1; break;
      case TextureFormat::RG:
      case TextureFormat::DEPTH_STENCIL: numberOfComponents = 2; break;
      case TextureFormat::RGB: numberOfComponents = 3; break;
      case TextureFormat::RGBA: numberOfComponents = 4; break;
    }

    std::visit(TextureResolutionVisitor{ resolutionMultiplied }, size);

    switch (dataType)
    {
      using enum TextureDataType;
      case UNSIGNED_BYTE: bytesPerComponent = 1; break;
      case UNSIGNED_SHORT: bytesPerComponent = 2; break;
      case FLOAT:
      case UNSIGNED_INT:
      case INT: bytesPerComponent = 4; break;
    }

    size_t memory =
      resolutionMultiplied * numberOfComponents * bytesPerComponent * samples;

    return memory;
  }

  size_t
  CalculateFramebufferMemory(const FramebufferSpecification& specification)
  {
    size_t memory = 0;

    for (auto attachment : specification.Attachments.Attachments)
    {
      size_t bytesPerPixel = 0;

      switch (attachment.TextureFormat)
      {
        using enum FramebufferTextureFormat;
        case None: break;
        case RGBA16F: bytesPerPixel = 8; break;
        case RED_INTEGER:
        case RGBA8:
        case DEPTH:
        case STENCIL:
        case DEPTH24STENCIL8: bytesPerPixel = 4; break;
      }

      memory += bytesPerPixel * specification.Height * specification.Width *
                specification.Samples;
    }

    return memory;
  }

  void
  VramTracker::AddTextureMemory(size_t bytes)
  {
    mTextureMemory += bytes;
  }

  size_t
  VramTracker::AddTextureMemory(const TextureType&       type,
                                const TextureFormat&     format,
                                const TextureDataType&   dataType,
                                const TextureResolution& size,
                                int                      samples)
  {
    size_t memory =
      CalculateTextureMemory(type, format, dataType, size, samples);

    mTextureMemory += memory;

    return memory;
  }

  size_t
  VramTracker::AddTextureMemory(std::shared_ptr<TextureContainer> texture)
  {
    size_t memory = CalculateTextureMemory(texture->Type,
                                           texture->Format,
                                           texture->DataType,
                                           texture->Size,
                                           texture->Samples);

    mTextureMemory += memory;

    return memory;
  }

  void
  VramTracker::RemoveTextureMemory(size_t bytes)
  {
    mTextureMemory -= bytes;
  }

  void
  VramTracker::RemoveTextureMemory(const TextureType&       type,
                                   const TextureFormat&     format,
                                   const TextureDataType&   dataType,
                                   const TextureResolution& size,
                                   int                      samples)
  {
    size_t memory =
      CalculateTextureMemory(type, format, dataType, size, samples);

    mTextureMemory -= memory;
  }

  void
  VramTracker::RemoveTextureMemory(std::shared_ptr<TextureContainer> texture)
  {
    size_t memory = CalculateTextureMemory(texture->Type,
                                           texture->Format,
                                           texture->DataType,
                                           texture->Size,
                                           texture->Samples);

    mTextureMemory -= memory;
  }

  size_t
  VramTracker::GetTextureMemory() const
  {
    return mTextureMemory;
  }

  void
  VramTracker::AddBufferMemory(size_t bytes)
  {
    mBufferMemory += bytes;
  }

  size_t
  VramTracker::AddFramebufferMemory(FramebufferSpecification specification)
  {
    size_t memory = CalculateFramebufferMemory(specification);

    mFramebufferMemory += memory;

    return memory;
  }

  void
  VramTracker::RemoveBufferMemory(size_t bytes)
  {
    mBufferMemory -= bytes;
  }

  void
  VramTracker::RemoveFramebufferMemory(FramebufferSpecification specification)
  {
    size_t memory = CalculateFramebufferMemory(specification);

    mFramebufferMemory -= memory;
  }

  size_t
  VramTracker::GetBufferMemory() const
  {
    return mBufferMemory;
  }

  void
  VramTracker::AddFramebufferMemory(size_t bytes)
  {
    mFramebufferMemory += bytes;
  }

  void
  VramTracker::RemoveFramebufferMemory(size_t bytes)
  {
    mFramebufferMemory -= bytes;
  }

  size_t
  VramTracker::GetFramebufferMemory() const
  {
    return mFramebufferMemory;
  }

  void
  VramTracker::AddShaderMemory(size_t bytes)
  {
    mShaderMemory += bytes;
  }

  void
  VramTracker::RemoveShaderMemory(size_t bytes)
  {
    mShaderMemory -= bytes;
  }

  size_t
  VramTracker::GetShaderMemory() const
  {
    return mShaderMemory;
  }

  void
  VramTracker::AddComputeMemory(size_t bytes)
  {
    mComputeMemory += bytes;
  }

  void
  VramTracker::RemoveComputeMemory(size_t bytes)
  {
    mComputeMemory -= bytes;
  }

  size_t
  VramTracker::GetComputeMemory() const
  {
    return mComputeMemory;
  }

  size_t
  VramTracker::GetTotalMemory() const
  {
    return mTextureMemory + mBufferMemory + mFramebufferMemory + mShaderMemory +
           mComputeMemory;
  }
}