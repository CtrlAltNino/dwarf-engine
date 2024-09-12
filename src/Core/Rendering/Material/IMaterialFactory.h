#pragma once

#include "IMaterial.h"

namespace Dwarf
{
  class IMaterialFactory
  {
  public:
    virtual ~IMaterialFactory() = default;

    virtual std::unique_ptr<IMaterial>
    CreateDefaultMaterial() const = 0;

    virtual std::unique_ptr<IMaterial>
    CreateMaterial(std::shared_ptr<IShader> shader) const = 0;

    virtual std::unique_ptr<IMaterial>
    FromSerialized(const nlohmann::json& serializedMaterial) const = 0;
  };
} // namespace Dwarf