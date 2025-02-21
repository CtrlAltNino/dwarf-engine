#pragma once

#include "Core/Rendering/Texture/ITexture.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include <memory>

namespace Dwarf
{
  class ITextureFactory
  {
  public:
    virtual ~ITextureFactory() = default;

    virtual std::unique_ptr<ITexture>
    FromPath(std::filesystem::path texturePath) = 0;

    virtual std::unique_ptr<ITexture>
    FromData(const std::shared_ptr<TextureContainer>& textureData) = 0;

    virtual std::unique_ptr<ITexture>
    Empty(const TextureType&       type,
          const TextureFormat&     format,
          const TextureDataType&   dataType,
          const TextureResolution& size,
          const TextureParameters& parameters,
          int                      samples = 1) = 0;

    virtual std::unique_ptr<ITexture>
    Empty(const TextureType&       type,
          const TextureFormat&     format,
          const TextureDataType&   dataType,
          const TextureResolution& size,
          int                      samples = 1) = 0;

    virtual std::shared_ptr<ITexture>
    GetPlaceholderTexture() = 0;
  };
} // namespace Dwarf