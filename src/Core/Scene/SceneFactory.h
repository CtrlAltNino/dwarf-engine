#pragma once

#include "Core/Scene/Properties/IScenePropertiesFactory.h"
#include "ISceneFactory.h"

namespace Dwarf
{
  /// @brief Factory for creating scenes.
  class SceneFactory : public ISceneFactory
  {
  private:
    std::shared_ptr<IScenePropertiesFactory> m_ScenePropertiesFactory;

    std::string
    CreateNewSceneName(const std::filesystem::path& directory);

  public:
    SceneFactory(
      std::shared_ptr<IScenePropertiesFactory> scenePropertiesFactory);

    std::unique_ptr<IScene>
    FromAsset(IAssetReference<SceneAsset>& sceneAsset) override;

    std::unique_ptr<IScene>
    NewSceneAsset(const std::filesystem::path& directory) override;

    std::unique_ptr<IScene>
    NewEmptyScene() override;
  };
} // namespace Dwarf