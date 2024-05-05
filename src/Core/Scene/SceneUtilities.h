#pragma once

#include <nlohmann/json.hpp>

#include "Core/Scene/SceneComponents.h"
#include "Core/Scene/Scene.h"
#include "Core/Base.h"
#include "Utilities/FileHandler.h"

namespace Dwarf
{

  /// @brief This class provides functionality to load and save scenes.
  class SceneUtilities
  {
  private:
    /// @brief Loads a scene from a path.
    /// @param path Path to the scene asset.
    /// @return The loaded scene.
    static std::shared_ptr<Scene>
    Deserialize(std::filesystem::path const& path);

    static void
    Serialize(std::shared_ptr<Scene> scene);

    static nlohmann::json
    SerializeEntities(std::vector<entt::entity> const& entities,
                      std::shared_ptr<Scene>           scene);

    static Entity
    DeserializeEntity(nlohmann::json         serializedEntity,
                      std::shared_ptr<Scene> scene);

  public:
    static bool
    SaveScene(std::shared_ptr<Scene> scene);

    static bool
    SaveSceneDialog(std::shared_ptr<Scene> scene);

    static std::shared_ptr<Scene>
    LoadScene(std::filesystem::path const& path);

    static std::shared_ptr<Scene>
    LoadSceneDialog();

    static std::shared_ptr<Scene>
    LoadDefaultScene();

    static void
    SetLastOpenedScene(std::filesystem::path const& path);
  };
}