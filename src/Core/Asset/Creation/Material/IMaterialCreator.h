#pragma once

#include "Core/Rendering/Material/IMaterial.h"

namespace Dwarf
{
  class IMaterialCreator
  {
  public:
    virtual ~IMaterialCreator() = default;

    virtual void
    CreateMaterialAsset(
      std::optional<std::filesystem::path> assetPath = std::nullopt) = 0;
  };
} // namespace Dwarf