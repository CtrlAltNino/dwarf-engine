#include "IMaterialCreator.h"
#include "pch.h"

#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "MaterialCreator.h"

#include <utility>

namespace Dwarf
{
  MaterialCreator::MaterialCreator(
    const AssetDirectoryPath&         assetDirectoryPath,
    std::shared_ptr<IMaterialFactory> materialFactory,
    std::shared_ptr<IMaterialIO>      materialIO,
    std::shared_ptr<IFileHandler>     fileHandler,
    std::shared_ptr<IShaderSourceCollectionFactory>
      shaderSourceCollectionFactory)
    : mAssetDirectoryPath(assetDirectoryPath)
    , mMaterialFactory(std::move(materialFactory))
    , mMaterialIo(std::move(materialIO))
    , mFileHandler(std::move(fileHandler))
    , mShaderSourceCollectionFactory(std::move(shaderSourceCollectionFactory))
  {
  }

  void
  MaterialCreator::CreateMaterialAsset(
    MaterialType                         materialType,
    std::optional<std::filesystem::path> directoryPath)
  {
    std::unique_ptr<IShaderSourceCollection> shaderSourceCollection;
    std::unique_ptr<IMaterial>               newMaterial;

    switch (materialType)
    {
      using enum MaterialType;
      case PbrMaterial:
        shaderSourceCollection =
          mShaderSourceCollectionFactory->CreatePbrShaderSourceCollection();
        break;
      case UnlitMaterial:
        shaderSourceCollection =
          mShaderSourceCollectionFactory->CreateUnlitShaderSourceCollection();
        break;
    }

    newMaterial =
      mMaterialFactory->CreateMaterial(std::move(shaderSourceCollection));

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