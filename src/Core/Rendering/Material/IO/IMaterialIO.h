#pragma once

#include "Core/Rendering/Material/IMaterial.h"

namespace Dwarf
{
  class IMaterialIO
  {
  public:
    virtual ~IMaterialIO() = default;

    virtual void
    SaveMaterial(std::shared_ptr<IMaterial>   material,
                 std::filesystem::path const& path) = 0;

    virtual std::shared_ptr<IMaterial>
    LoadMaterial(std::filesystem::path const& path) = 0;
  };
};