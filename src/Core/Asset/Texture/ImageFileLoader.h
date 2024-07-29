#pragma once

#include "IImageFileLoader.h"

namespace Dwarf
{
  class ImageFileLoader : public IImageFileLoader
  {
  public:
    std::shared_ptr<TextureContainer>
    LoadTexture(std::filesystem::path texturePath) override;
  };
} // namespace Dwarf