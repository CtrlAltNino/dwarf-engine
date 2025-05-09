#pragma once

#include "Core/Rendering/CubemapGenerator/ICubemapGenerator.hpp"

namespace Dwarf
{
  class ICubemapGeneratorFactory
  {
  public:
    virtual ~ICubemapGeneratorFactory() = default;

    [[nodiscard]] virtual auto
    Create() -> std::shared_ptr<ICubemapGenerator> = 0;
  };
}