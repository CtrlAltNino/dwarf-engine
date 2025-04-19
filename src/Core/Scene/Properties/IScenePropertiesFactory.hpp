#pragma once

#include "Core/Asset/AssetReference/IAssetReference.hpp"
#include "Core/Scene/Properties/ISceneProperties.hpp"

namespace Dwarf
{
  /**
   * @brief Factory class that creates ScneeProperties instances
   *
   */
  class IScenePropertiesFactory
  {
  public:
    virtual ~IScenePropertiesFactory() = default;

    /**
     * @brief Creates an empty SceneProperties instance with just a provided
     * name
     *
     * @param name Name for the scene
     * @return Unique pointer to the created SceneProperties instance
     */
    [[nodiscard]] virtual auto
    Create(const std::string& name) const
      -> std::unique_ptr<ISceneProperties> = 0;

    /**
     * @brief Creates a SceneProperties for a serialized scene asset
     *
     * @param sceneAsset Scene asset
     * @param serializedProperties Serialized scene properties
     * @return Unique pointer to the created SceneProperties instance
     */
    [[nodiscard]] virtual auto
    Create(IAssetReference&      sceneAsset,
           const nlohmann::json& serializedProperties) const
      -> std::unique_ptr<ISceneProperties> = 0;
  };
}