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
    std::shared_ptr<IAssetDatabase>   m_assetDatabase;
    std::shared_ptr<IMaterialFactory> m_materialFactory;
    std::shared_ptr<IMaterialIO>      m_materialIO;

  public:
    MaterialCreator(std::shared_ptr<IAssetDatabase>   assetDatabase,
                    std::shared_ptr<IMaterialFactory> materialFactory,
                    std::shared_ptr<IMaterialIO>      materialIO);
    void
    CreateMaterialAsset(
      std::optional<std::filesystem::path> assetPath) override;
  };
} // namespace Dwarf