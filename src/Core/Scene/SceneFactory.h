#pragma once

#include "Core/Scene/Properties/IScenePropertiesFactory.h"
#include "ISceneFactory.h"

namespace Dwarf
{
  /// @brief Factory for creating scenes.
  class SceneFactory : public ISceneFactory
  {
  public:
    SceneFactory(
      std::shared_ptr<IScenePropertiesFactory> scenePropertiesFactory);

    std::shared_ptr<IScene>
    FromAsset(std::shared_ptr<AssetReference<SceneAsset>> sceneAsset) override;

    std::shared_ptr<IScene>
    NewAsset(std::filesystem::path directory) override;

    std::shared_ptr<IScene>
    NewEmpty() override;

  private:
    std::shared_ptr<IScenePropertiesFactory> m_ScenePropertiesFactory;

    std::string
    CreateNewSceneName(std::filesystem::path directory);
  };
} // namespace Dwarf