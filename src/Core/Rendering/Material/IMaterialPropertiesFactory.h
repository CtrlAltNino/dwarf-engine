#pragma once

#include "IMaterialProperties.h"

namespace Dwarf
{
  class IMaterialPropertiesFactory
  {
  public:
    virtual ~IMaterialPropertiesFactory() = default;

    virtual std::shared_ptr<IMaterialProperties>
    CreateMaterialProperties() = 0;

    virtual std::shared_ptr<IMaterialProperties>
    CreateMaterialProperties(
      const nlohmann::json& serializedMaterialProperties) = 0;
  };
} // namespace Dwarf