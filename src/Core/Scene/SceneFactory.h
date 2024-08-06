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
    FromAsset(const AssetReference<SceneAsset>& sceneAsset) override;

    std::shared_ptr<IScene>
    New(std::filesystem::path directory) override;

  private:
    std::shared_ptr<IScenePropertiesFactory> m_ScenePropertiesFactory;

    std::string
    CreateNewSceneName(std::filesystem::path directory);
  };
} // namespace Dwarf