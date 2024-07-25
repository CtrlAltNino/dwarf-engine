#pragma once

#include "Utilities/ImageUtilities/TextureCommon.h"

namespace Dwarf
{
  class ITextureLoader
  {
  public:
    virtual ~ITextureLoader() = default;

    virtual std::shared_ptr<TextureContainer>
    LoadTexture(std::filesystem::path texturePath) = 0;
  };
} // namespace Dwarf