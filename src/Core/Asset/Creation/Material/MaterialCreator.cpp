#include "MaterialCreator.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include <memory>

namespace Dwarf
{
  MaterialCreator::MaterialCreator(
    AssetDirectoryPath                assetDirectoryPath,
    std::shared_ptr<IMaterialFactory> materialFactory,
    std::shared_ptr<IMaterialIO>      materialIO,
    std::shared_ptr<IFileHandler>     fileHandler)
    : m_AssetDirectoryPath(assetDirectoryPath)
    , m_MaterialFactory(materialFactory)
    , m_MaterialIO(materialIO)
    , m_FileHandler(fileHandler)
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

    while (m_FileHandler->FileExists(assetPath))
    {
      static int counter = 0;

      assetPath =
        path / ("New Material (" + std::to_string(counter) + ").dmat");

      counter++;
    }

    m_MaterialIO->SaveMaterial(*newMaterial, assetPath);
  }
} // namespace Dwarf