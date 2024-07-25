#pragma once

#include "Core/Rendering/Texture/ITexture.h"

namespace Dwarf
{
  class ITextureFactory
  {
  public:
    virtual ~ITextureFactory() = default;

    virtual std::shared_ptr<ITexture>
    CreateTexture(std::filesystem::path texturePath) = 0;

    virtual std::shared_ptr<ITexture>
    CreateTexture(const TextureContainer& textureData) = 0;

    virtual std::shared_ptr<ITexture>
    CreateEmptyTexture(const TextureContainer& textureData) = 0;
  };
} // namespace Dwarf