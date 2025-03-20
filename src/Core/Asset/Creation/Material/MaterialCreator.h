#pragma once

#include "Core/Asset/Creation/Material/IMaterialCreator.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Utilities/FileHandler/IFileHandler.h"

namespace Dwarf
{
  class MaterialCreator : public IMaterialCreator
  {
  private:
    AssetDirectoryPath                m_AssetDirectoryPath;
    std::shared_ptr<IMaterialFactory> m_MaterialFactory;
    std::shared_ptr<IMaterialIO>      m_MaterialIO;
    std::shared_ptr<IFileHandler>     m_FileHandler;

  public:
    MaterialCreator(const AssetDirectoryPath&         assetDirectoryPath,
                    std::shared_ptr<IMaterialFactory> materialFactory,
                    std::shared_ptr<IMaterialIO>      materialIO,
                    std::shared_ptr<IFileHandler>     fileHandler);
    ~MaterialCreator() override = default;

    void
    CreateMaterialAsset(
      std::optional<std::filesystem::path> directoryPath) override;
  };
} // namespace Dwarf