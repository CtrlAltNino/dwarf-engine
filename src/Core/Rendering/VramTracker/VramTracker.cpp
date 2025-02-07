#include "VramTracker.h"
#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Utilities/ImageUtilities/TextureCommon.h"

namespace Dwarf
{
  struct TextureResolutionVisitor
  {
    size_t& m_ResolutionMultiplied;

    void
    operator()(const glm::ivec1& parameter)
    {
      m_ResolutionMultiplied = parameter.x;
    }

    void
    operator()(const glm::ivec2& parameter)
    {
      m_ResolutionMultiplied = parameter.x * parameter.y;
    }

    void
    operator()(const glm::ivec3& parameter)
    {
      m_ResolutionMultiplied = parameter.x * parameter.y * parameter.z;
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
      case Dwarf::TextureDataType::UNSIGNED_BYTE: bytesPerComponent = 1; break;
      case Dwarf::TextureDataType::UNSIGNED_SHORT: bytesPerComponent = 2; break;
      case Dwarf::TextureDataType::FLOAT:
      case Dwarf::TextureDataType::UNSIGNED_INT:
      case Dwarf::TextureDataType::INT: bytesPerComponent = 4; break;
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
        case Dwarf::FramebufferTextureFormat::RED_INTEGER:
        case Dwarf::FramebufferTextureFormat::RGBA8:
        case Dwarf::FramebufferTextureFormat::DEPTH:
        case Dwarf::FramebufferTextureFormat::STENCIL:
        case Dwarf::FramebufferTextureFormat::DEPTH24STENCIL8:
          bytesPerPixel = 4;
          break;
      }

      memory += bytesPerPixel * specification.Height * specification.Width *
                specification.Samples;
    }

    return memory;
  }

  void
  VramTracker::AddTextureMemory(size_t bytes)
  {
    m_TextureMemory += bytes;
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

    m_TextureMemory += memory;

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

    m_TextureMemory += memory;

    return memory;
  }

  void
  VramTracker::RemoveTextureMemory(size_t bytes)
  {
    m_TextureMemory -= bytes;
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

    m_TextureMemory -= memory;
  }

  void
  VramTracker::RemoveTextureMemory(std::shared_ptr<TextureContainer> texture)
  {
    size_t memory = CalculateTextureMemory(texture->Type,
                                           texture->Format,
                                           texture->DataType,
                                           texture->Size,
                                           texture->Samples);

    m_TextureMemory -= memory;
  }

  size_t
  VramTracker::GetTextureMemory() const
  {
    return m_TextureMemory;
  }

  void
  VramTracker::AddBufferMemory(size_t bytes)
  {
    m_BufferMemory += bytes;
  }

  size_t
  VramTracker::AddFramebufferMemory(FramebufferSpecification specification)
  {
    size_t memory = CalculateFramebufferMemory(specification);

    m_FramebufferMemory += memory;

    return memory;
  }

  void
  VramTracker::RemoveBufferMemory(size_t bytes)
  {
    m_BufferMemory -= bytes;
  }

  void
  VramTracker::RemoveFramebufferMemory(FramebufferSpecification specification)
  {
    size_t memory = CalculateFramebufferMemory(specification);

    m_FramebufferMemory -= memory;
  }

  size_t
  VramTracker::GetBufferMemory() const
  {
    return m_BufferMemory;
  }

  void
  VramTracker::AddFramebufferMemory(size_t bytes)
  {
    m_FramebufferMemory += bytes;
  }

  void
  VramTracker::RemoveFramebufferMemory(size_t bytes)
  {
    m_FramebufferMemory -= bytes;
  }

  size_t
  VramTracker::GetFramebufferMemory() const
  {
    return m_FramebufferMemory;
  }

  void
  VramTracker::AddShaderMemory(size_t bytes)
  {
    m_ShaderMemory += bytes;
  }

  void
  VramTracker::RemoveShaderMemory(size_t bytes)
  {
    m_ShaderMemory -= bytes;
  }

  size_t
  VramTracker::GetShaderMemory() const
  {
    return m_ShaderMemory;
  }

  void
  VramTracker::AddComputeMemory(size_t bytes)
  {
    m_ComputeMemory += bytes;
  }

  void
  VramTracker::RemoveComputeMemory(size_t bytes)
  {
    m_ComputeMemory -= bytes;
  }

  size_t
  VramTracker::GetComputeMemory() const
  {
    return m_ComputeMemory;
  }

  size_t
  VramTracker::GetTotalMemory() const
  {
    return m_TextureMemory + m_BufferMemory + m_FramebufferMemory +
           m_ShaderMemory + m_ComputeMemory;
  }
}