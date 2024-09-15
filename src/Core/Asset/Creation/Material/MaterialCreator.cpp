#include "MaterialCreator.h"
#include "Utilities/FileHandler.h"
#include <memory>

namespace Dwarf
{
  MaterialCreator::MaterialCreator(
    AssetDirectoryPath                assetDirectoryPath,
    std::shared_ptr<IMaterialFactory> materialFactory,
    std::shared_ptr<IMaterialIO>      materialIO)
    : m_AssetDirectoryPath(assetDirectoryPath)
    , m_MaterialFactory(materialFactory)
    , m_MaterialIO(materialIO)
  {
  }

  void
  MaterialCreator::CreateMaterialAsset(
    std::optional<std::filesystem::path> directory)
  {
    std::unique_ptr<IMaterial> newMaterial =
      m_MaterialFactory->CreateDefaultMaterial();

    std::filesystem::path path =
      directory.has_value() ? directory.value() : m_AssetDirectoryPath.t;

    std::filesystem::path assetPath = path / "New Material.dmat";

    while (FileHandler::FileExists(assetPath))
    {
      static int counter = 0;

      assetPath =
        path / ("New Material (" + std::to_string(counter) + ").dmat");
    }

    m_MaterialIO->SaveMaterial(*newMaterial, assetPath);
  }
} // namespace Dwarf