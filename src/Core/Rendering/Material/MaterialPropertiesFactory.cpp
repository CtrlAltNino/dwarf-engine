#include "MaterialPropertiesFactory.h"

#include "MaterialProperties.h"

namespace Dwarf
{
  std::shared_ptr<IMaterialProperties>
  MaterialPropertiesFactory::CreateMaterialProperties()
  {
    return std::make_shared<MaterialProperties>(false, false, false, false);
  }

  std::shared_ptr<IMaterialProperties>
  MaterialPropertiesFactory::CreateMaterialProperties(
    const nlohmann::json& serializedMaterialProperties)
  {
    return std::make_shared<MaterialProperties>(
      serializedMaterialProperties["IsTransparent"],
      serializedMaterialProperties["IsDoubleSided"],
      serializedMaterialProperties["IsUnlit"],
      serializedMaterialProperties["IsWireframe"]);
  }
} // namespace Dwarf