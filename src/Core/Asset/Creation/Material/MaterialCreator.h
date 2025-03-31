#pragma once

#include "Core/Asset/Creation/Material/IMaterialCreator.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Utilities/FileHandler/IFileHandler.h"

namespace Dwarf
{
  class MaterialCreator : public IMaterialCreator
  {
  private:
    AssetDirectoryPath                mAssetDirectoryPath;
    std::shared_ptr<IMaterialFactory> mMaterialFactory;
    std::shared_ptr<IMaterialIO>      mMaterialIo;
    std::shared_ptr<IFileHandler>     mFileHandler;
    std::shared_ptr<IShaderSourceCollectionFactory>
      mShaderSourceCollectionFactory;

  public:
    MaterialCreator(const AssetDirectoryPath&         assetDirectoryPath,
                    std::shared_ptr<IMaterialFactory> materialFactory,
                    std::shared_ptr<IMaterialIO>      materialIO,
                    std::shared_ptr<IFileHandler>     fileHandler,
                    std::shared_ptr<IShaderSourceCollectionFactory>
                      shaderSourceCollectionFactory);
    ~MaterialCreator() override = default;

    void
    CreateMaterialAsset(
      MaterialType                         materialType,
      std::optional<std::filesystem::path> directoryPath) override;
  };
} // namespace Dwarf