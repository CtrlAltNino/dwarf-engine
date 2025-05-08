#pragma once

#include "Core/Rendering/CubeMapGenerator/ICubeMapGenerator.hpp"
namespace Dwarf
{
  class ICubeMapGeneratorFactory
  {
  public:
    virtual ~ICubeMapGeneratorFactory() = default;

    [[nodiscard]] virtual auto
    Create() -> std::shared_ptr<ICubeMapGenerator> = 0;
  };
}