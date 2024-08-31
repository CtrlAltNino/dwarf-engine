#pragma once

#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Database/AssetReference.h"
#include "Core/Scene/Settings/ISceneSettings.h"
#include "Utilities/ISerializable.h"
namespace Dwarf
{
  /// @brief Interface for scene properties.
  class ISceneProperties : public ISerializable
  {
  public:
    /// @brief Destructor.
    virtual ~ISceneProperties() = default;

    /// @brief Returns the name of the scene.
    /// @return The name of the scene.
    virtual std::string
    GetName() const = 0;

    /// @brief Returns the asset reference of the scene.
    /// @return The asset reference of the scene.
    virtual std::shared_ptr<AssetReference<SceneAsset>>
    GetAsset() const = 0;

    /// @brief Returns the settings of the scene.
    /// @return The settings of the scene.
    virtual std::shared_ptr<ISceneSettings>
    GetSettings() const = 0;
  };
}