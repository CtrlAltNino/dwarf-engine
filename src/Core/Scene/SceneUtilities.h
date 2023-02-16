#pragma once

#include "Core/Scene/Scene.h"

namespace Dwarf {

    /// @brief This class provides functionality to load and save scenes.
    class SceneUtilities {
        public:
            /// @brief Loads a scene from a path.
            /// @param scenePath Path to the scene asset.
            /// @return The loaded scene.
            void LoadScene(Scene* scene, std::filesystem::path scenePath);
    };
}