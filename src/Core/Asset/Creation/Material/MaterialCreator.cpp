#include "MaterialCreator.h"
#include "Utilities/FileHandler.h"
#include <memory>

namespace Dwarf
{
  MaterialCreator::MaterialCreator(
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IMaterialFactory> materialFactory,
    std::shared_ptr<IMaterialIO>      materialIO)
    : m_assetDatabase(assetDatabase)
    , m_materialFactory(materialFactory)
    , m_materialIO(materialIO)
  {
  }

  void
  MaterialCreator::CreateMaterialAsset(
    std::optional<std::filesystem::path> directory)
  {
    std::shared_ptr<IMaterial> newMaterial =
      m_materialFactory->CreateDefaultMaterial();

    std::filesystem::path path = directory.has_value()
                                   ? directory.value()
                                   : m_assetDatabase->GetAssetDirectoryPath();

    std::filesystem::path assetPath = path / "New Material.dmat";

    while (FileHandler::FileExists(assetPath))
    {
      static int counter = 0;

      assetPath =
        path / ("New Material (" + std::to_string(counter) + ").dmat");
    }

    m_materialIO->SaveMaterial(newMaterial, assetPath);
  }
} // namespace Dwarf