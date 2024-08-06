#pragma once

#include "IMaterialPropertiesFactory.h"

namespace Dwarf
{
  class MaterialPropertiesFactory : public IMaterialPropertiesFactory
  {
  public:
    std::shared_ptr<IMaterialProperties>
    CreateMaterialProperties() override;

    std::shared_ptr<IMaterialProperties>
    CreateMaterialProperties(
      const nlohmann::json& serializedMaterialProperties) override;
  };
} // namespace Dwarf