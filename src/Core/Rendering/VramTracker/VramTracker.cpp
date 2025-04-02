#include "pch.h"

#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include "VramTracker.h"

namespace Dwarf
{
  /**
   * @brief Visitor struct to handle the resolution variants
   *
   */
  struct TextureResolutionVisitor
  {
    std::reference_wrapper<size_t> mResolutionMultiplied;

    void
    operator()(const glm::ivec1& parameter) const
    {
      mResolutionMultiplied.get() = parameter.x;
    }

    void
    operator()(const glm::ivec2& parameter) const
    {
      mResolutionMultiplied.get() =
        static_cast<long long>(parameter.x) * parameter.y;
    }

    void
    operator()(const glm::ivec3& parameter) const
    {
      mResolutionMultiplied.get() = static_cast<unsigned long long>(
        parameter.x * parameter.y * parameter.z);
    }
  };

  auto
  CalculateTextureMemory(const TextureFormat&     format,
                         const TextureDataType&   dataType,
                         const TextureResolution& size,
                         int                      samples) -> size_t
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

  auto
  CalculateFramebufferMemory(const FramebufferSpecification& specification)
    -> size_t
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
        case SRGBA8:
        case DEPTH:
        case STENCIL:
        case DEPTH24STENCIL8: bytesPerPixel = 4; break;
      }

      memory += bytesPerPixel * specification.Height * specification.Width *
                specification.Samples;
    }

    return memory;
  }

  VramTracker::VramTracker(std::shared_ptr<IDwarfLogger> logger)
    : mLogger(std::move(logger))
  {
    mLogger->LogDebug(Log("VramTracker created.", "VramTracker"));
  }
  VramTracker::~VramTracker()
  {
    mLogger->LogDebug(Log("VramTracker destroyed.", "VramTracker"));
  }

  void
  VramTracker::AddTextureMemory(size_t bytes)
  {
    mTextureMemory += bytes;
  }

  auto
  VramTracker::AddTextureMemory(const TextureFormat&     format,
                                const TextureDataType&   dataType,
                                const TextureResolution& size,
                                int                      samples) -> size_t
  {
    size_t memory = CalculateTextureMemory(format, dataType, size, samples);

    mTextureMemory += memory;

    return memory;
  }

  auto
  VramTracker::AddTextureMemory(std::shared_ptr<TextureContainer> texture)
    -> size_t
  {
    size_t   memory = 0;
    uint32_t mipLevels =
      texture->Parameters.MipMapped
        ? CalculateMipLevels(std::get<glm::ivec2>(texture->Size))
        : 1;

    if (mipLevels > 1)
    {
      glm::ivec2 currentResolution = std::get<glm::ivec2>(texture->Size);
      for (int i = 0; i < mipLevels; i++)
      {
        memory += CalculateTextureMemory(texture->Format,
                                         texture->DataType,
                                         currentResolution,
                                         texture->Samples);
        currentResolution /= 2;
      }
    }
    else
    {
      memory += CalculateTextureMemory(
        texture->Format, texture->DataType, texture->Size, texture->Samples);
    }

    mTextureMemory += memory;

    return memory;
  }

  void
  VramTracker::RemoveTextureMemory(size_t bytes)
  {
    mTextureMemory -= bytes;
  }

  void
  VramTracker::RemoveTextureMemory(const TextureFormat&     format,
                                   const TextureDataType&   dataType,
                                   const TextureResolution& size,
                                   int                      samples)
  {
    size_t memory = CalculateTextureMemory(format, dataType, size, samples);

    mTextureMemory -= memory;
  }

  void
  VramTracker::RemoveTextureMemory(std::shared_ptr<TextureContainer> texture)
  {
    size_t   memory = 0;
    uint32_t mipLevels =
      texture->Parameters.MipMapped
        ? CalculateMipLevels(std::get<glm::ivec2>(texture->Size))
        : 1;

    if (mipLevels > 1)
    {
      glm::ivec2 currentResolution = std::get<glm::ivec2>(texture->Size);
      for (int i = 0; i < mipLevels; i++)
      {
        memory += CalculateTextureMemory(texture->Format,
                                         texture->DataType,
                                         currentResolution,
                                         texture->Samples);
        currentResolution /= 2;
      }
    }
    else
    {
      memory += CalculateTextureMemory(
        texture->Format, texture->DataType, texture->Size, texture->Samples);
    }

    mTextureMemory -= memory;
  }

  auto
  VramTracker::GetTextureMemory() const -> size_t
  {
    return mTextureMemory;
  }

  void
  VramTracker::AddBufferMemory(size_t bytes)
  {
    mBufferMemory += bytes;
  }

  auto
  VramTracker::AddFramebufferMemory(FramebufferSpecification specification)
    -> size_t
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

  auto
  VramTracker::GetBufferMemory() const -> size_t
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

  auto
  VramTracker::GetFramebufferMemory() const -> size_t
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

  auto
  VramTracker::GetShaderMemory() const -> size_t
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

  auto
  VramTracker::GetComputeMemory() const -> size_t
  {
    return mComputeMemory;
  }

  auto
  VramTracker::GetTotalMemory() const -> size_t
  {
    return mTextureMemory + mBufferMemory + mFramebufferMemory + mShaderMemory +
           mComputeMemory;
  }
}