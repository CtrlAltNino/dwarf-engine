#pragma once

#include "Editor/Modules/IModule.h"
#include "Editor/EditorCommon.h"
#include "Editor/Modules/Scene Hierarchy/GraphInstruction.h"
#include "Editor/Modules/Scene Hierarchy/NewParentInstruction.h"
#include "Editor/Modules/Scene Hierarchy/ChildIndexInstruction.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/EntityProvider.h"
#include "Input/IInputManager.h"

namespace Dwarf {

    /// @brief Module to display and manipulate the scene graph.
    class SceneGraphModule : public IModule {
        private:
            /// @brief Pointer to the currently opened scene.
            Scene *scene;

            /// @brief Pointer to the input manager.
            IInputManager* inputManager;

            /// @brief List of graph instruction. Used as a buffer, executed at the end of a frame.
            std::vector<GraphInstruction*> instructions;

            /// @brief Draws a graph node for an entity.
            /// @param entity The entity to draw the node of.
            void DrawNode(entt::entity entity);

            /// @brief Executes all buffered instructions.
            void ProcessInstructions();
        public:
            SceneGraphModule(IViewListener *listener, Scene *scene, IInputManager* inputManager, int index);

            /// @brief Renders the module window.
            void RenderModuleWindow() override;

            /// @brief Executes all pre frame tasks.
            void StartFrame() override {}
    };
}