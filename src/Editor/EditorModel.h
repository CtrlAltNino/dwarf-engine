#pragma once

#include "Core/Scene/Entity.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/MeshLoader.h"
#include "Editor/EditorCommon.h"
#include "Utilities/Common.h"

namespace Dwarf {

    /// @brief Model of the Editor's MVC structure.
    class EditorModel{
        private:
            /// @brief The currently opened scene instance.
            Scene scene;

            GraphicsApi activeApi;

            /// @brief Interfaced controller instance to communicate to the controller.
            IModelListener* modelListener;
        public:
            EditorModel(IModelListener* listener);

            /// @brief DEBUG initializes the scene.
            /// @param projectPath Path to the currently opened project.
            void InitScene(std::string projectPath);

            /// @brief Returns the currently opened scene.
            /// @return A pointer to the scene.
            Scene* GetScene();
    };
}