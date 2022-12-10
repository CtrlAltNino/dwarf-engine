#pragma once

#include"EditorCommon.h"
#include"../data structures/Entity.h"
#include"../scene/Scene.h"
#include"../scene/MeshLoader.h"

/// @brief Model of the Editor's MVC structure.
class EditorModel{
    private:
        /// @brief The currently opened scene instance.
        Scene scene;

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