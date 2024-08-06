#pragma once

#include "Core/Asset/Database/AssetReference.h"
#include "Core/Scene/IScene.h"

namespace Dwarf
{
  namespace SceneIO
  {
    class ISceneIO
    {
    public:
      virtual ~ISceneIO() {}

      virtual bool
      LoadScene(AssetReference<SceneAsset> sceneAsset) = 0;
      virtual bool
      SaveScene(const std::string& path, const IScene& scene) = 0;
    };
  }
}