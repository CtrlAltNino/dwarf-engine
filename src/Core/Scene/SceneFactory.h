#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Scene/Properties/IScenePropertiesFactory.h"
#include "ISceneFactory.h"

namespace Dwarf
{
  /// @brief Factory for creating scenes.
  class SceneFactory : public ISceneFactory
  {
  private:
    std::shared_ptr<IScenePropertiesFactory> m_ScenePropertiesFactory;
    std::shared_ptr<IAssetDatabase>          m_AssetDatabase;

  public:
    SceneFactory(
      std::shared_ptr<IScenePropertiesFactory> scenePropertiesFactory,
      std::shared_ptr<IAssetDatabase>          assetDatabase);

    std::unique_ptr<IScene>
    FromAsset(IAssetReference<SceneAsset>& sceneAsset) override;

    std::unique_ptr<IScene>
    CreateDefaultScene() override;
  };
} // namespace Dwarf