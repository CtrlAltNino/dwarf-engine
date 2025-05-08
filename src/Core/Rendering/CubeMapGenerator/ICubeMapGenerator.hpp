#pragma once

#include "Core/Rendering/Texture/ITexture.hpp"

namespace Dwarf
{
  class ICubeMapGenerator
  {
  public:
    virtual ~ICubeMapGenerator() = default;

    [[nodiscard]] virtual auto
    FromEquirectangular(std::shared_ptr<ITexture> texture, uint32_t resolution)
      -> std::shared_ptr<ITexture> = 0;
  };
}