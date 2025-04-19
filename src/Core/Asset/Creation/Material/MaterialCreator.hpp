#pragma once

#include "Core/Asset/Creation/Material/IMaterialCreator.hpp"
#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.hpp"
#include "Core/Rendering/Material/IMaterialFactory.hpp"
#include "Core/Rendering/Material/IO/IMaterialIO.hpp"
#include "Utilities/FileHandler/IFileHandler.hpp"

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