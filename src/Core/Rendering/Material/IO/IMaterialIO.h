#pragma once

#include "Core/Rendering/Material/IMaterial.h"

namespace Dwarf
{
  class IMaterialIO
  {
  public:
    virtual ~IMaterialIO() = default;

    virtual void
    SaveMaterial(IMaterial&                   material,
                 const std::filesystem::path& path) const = 0;

    virtual std::unique_ptr<IMaterial>
    LoadMaterial(const std::filesystem::path& path) const = 0;
  };
};