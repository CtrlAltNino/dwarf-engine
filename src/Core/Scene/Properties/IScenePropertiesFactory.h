#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Scene/Properties/ISceneProperties.h"

namespace Dwarf
{
  class IScenePropertiesFactory
  {
  public:
    virtual ~IScenePropertiesFactory() = default;

    virtual std::unique_ptr<ISceneProperties>
    Create(IAssetReference<SceneAsset>& sceneAsset,
           const nlohmann::json&        serializedProperties) = 0;

    virtual std::unique_ptr<ISceneProperties>
    Create(const std::string& name) = 0;
  };
}