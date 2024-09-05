#pragma once

#include "Core/Asset/Creation/Material/IMaterialCreator.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"

namespace Dwarf
{
  class MaterialCreator : public IMaterialCreator
  {
  private:
    AssetDirectoryPath                m_AssetDirectoryPath;
    std::shared_ptr<IMaterialFactory> m_MaterialFactory;
    std::shared_ptr<IMaterialIO>      m_MaterialIO;

  public:
    MaterialCreator(AssetDirectoryPath                assetDirectoryPath,
                    std::shared_ptr<IMaterialFactory> materialFactory,
                    std::shared_ptr<IMaterialIO>      materialIO);
    ~MaterialCreator() override = default;
    void
    CreateMaterialAsset(
      std::optional<std::filesystem::path> assetPath) override;
  };
} // namespace Dwarf