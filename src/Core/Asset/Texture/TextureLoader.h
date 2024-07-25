#pragma once

#include "ITextureLoader.h"

namespace Dwarf
{
  class TextureLoader : public ITextureLoader
  {
  public:
    std::shared_ptr<TextureContainer>
    LoadTexture(std::filesystem::path texturePath) override;
  };
} // namespace Dwarf