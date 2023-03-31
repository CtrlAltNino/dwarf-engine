#pragma once

#include <nlohmann/json.hpp>

#include "Core/Scene/SceneComponents.h"
#include "Core/Scene/Scene.h"
#include "Core/Base.h"
#include "Utilities/FileHandler.h"

namespace Dwarf {

    /// @brief This class provides functionality to load and save scenes.
    class SceneUtilities {
        private:
            /// @brief Loads a scene from a path.
            /// @param path Path to the scene asset.
            /// @return The loaded scene.
            static Ref<Scene> Deserialize(std::filesystem::path path);

            static void Serialize(Ref<Scene> scene);

            static nlohmann::json SerializeEntities(std::vector<entt::entity> entities, Ref<Scene> scene);

            static Entity DeserializeEntity(nlohmann::json serializedEntity, Ref<Scene> scene);
        public:
            static void SaveScene(Ref<Scene> scene);

            static void SaveSceneDialog(Ref<Scene> scene);

            static Ref<Scene> LoadScene(std::filesystem::path path);

            static Ref<Scene> LoadSceneDialog();

            static Ref<Scene> LoadDefaultScene();

            static void SetLastOpenedScene(std::filesystem::path path);
    };
}