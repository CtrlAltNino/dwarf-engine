#pragma once

#include "IMaterial.h"

namespace Dwarf
{

  class IMaterialFactory
  {
  public:
    virtual ~IMaterialFactory() = default;

    virtual std::shared_ptr<IMaterial>
    CreateMaterial() = 0;

    virtual std::shared_ptr<IMaterial>
    CreateMaterial(std::shared_ptr<IShader> shader) = 0;

    virtual std::shared_ptr<IMaterial>
    CreateMaterial(const nlohmann::json& serializedMaterial) = 0;
  };

} // namespace Dwarf