#include "pch.h"

#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "MaterialCreator.h"

namespace Dwarf
{
  MaterialCreator::MaterialCreator(
    const AssetDirectoryPath&         assetDirectoryPath,
    std::shared_ptr<IMaterialFactory> materialFactory,
    std::shared_ptr<IMaterialIO>      materialIO,
    std::shared_ptr<IFileHandler>     fileHandler)
    : mAssetDirectoryPath(assetDirectoryPath)
    , mMaterialFactory(std::move(materialFactory))
    , mMaterialIo(std::move(materialIO))
    , mFileHandler(std::move(fileHandler))
  {
  }

  void
  MaterialCreator::CreateMaterialAsset(
    std::optional<std::filesystem::path> directoryPath)
  {
    std::unique_ptr<IMaterial> newMaterial =
      mMaterialFactory->CreateDefaultMaterial();

    std::filesystem::path path =
      directoryPath.has_value() ? directoryPath.value() : mAssetDirectoryPath.t;

    std::filesystem::path assetPath = path / "New Material.dmat";

    while (mFileHandler->FileExists(assetPath))
    {
      static int counter = 0;

      assetPath =
        path / ("New Material (" + std::to_string(counter) + ").dmat");

      counter++;
    }

    mMaterialIo->SaveMaterial(*newMaterial, assetPath);
  }
} // namespace Dwarf