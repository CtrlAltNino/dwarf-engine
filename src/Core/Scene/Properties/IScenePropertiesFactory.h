#pragma once

#include "Core/Asset/Database/AssetReference.h"
#include "Core/Scene/Properties/ISceneProperties.h"

namespace Dwarf
{
  class IScenePropertiesFactory
  {
  public:
    virtual ~IScenePropertiesFactory() = default;

    virtual std::shared_ptr<ISceneProperties>
    Create(AssetReference<SceneAsset> sceneAsset,
           nlohmann::json             serializedProperties) = 0;

    virtual std::shared_ptr<ISceneProperties>
    Create(std::string name) = 0;
  };
}