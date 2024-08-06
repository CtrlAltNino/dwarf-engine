#pragma once
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Database/AssetReference.h"
#include "pch.h"
#include "Core/Scene/IScene.h"

namespace Dwarf
{
  /// @brief This class provides functionality to load and save scenes.
  class SceneUtilities
  {
  private:
    /// @brief Loads a scene from a path.
    /// @param path Path to the scene asset.
    /// @return The loaded scene.
    // static std::shared_ptr<Scene>
    // Deserialize(std::filesystem::path const& path);

    // static void
    // Serialize(std::shared_ptr<Scene> scene);

    // static nlohmann::json
    // SerializeEntities(std::vector<entt::entity> const& entities,
    //                   std::shared_ptr<Scene>           scene);

    // static Entity
    // DeserializeEntity(nlohmann::json         serializedEntity,
    //                   std::shared_ptr<Scene> scene);

  public:
    static bool
    SaveScene(std::shared_ptr<IScene> scene);

    static bool
    SaveSceneDialog(std::shared_ptr<IScene> scene);

    static std::shared_ptr<IScene>
    LoadScene(AssetReference<SceneAsset> sceneAsset);

    static std::shared_ptr<IScene>
    LoadSceneDialog();

    static std::shared_ptr<IScene>
    LoadDefaultScene();

    static void
    SetLastOpenedScene(AssetReference<SceneAsset> sceneAsset);
  };
}