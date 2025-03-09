#pragma once

#include "Utilities/ImageUtilities/TextureCommon.h"

namespace Dwarf
{
  class IImageFileLoader
  {
  public:
    virtual ~IImageFileLoader() = default;

    virtual std::shared_ptr<TextureContainer>
    LoadImageFile(std::filesystem::path texturePath) = 0;
  };
} // namespace Dwarf