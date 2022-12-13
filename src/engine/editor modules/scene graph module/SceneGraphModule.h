#pragma once

#include "../../../utilities/dpch.h"


#include"../IModule.h"
#include"../../scene/Scene.h"
#include"../../editor/EditorCommon.h"
#include"../../scene/EntityProvider.h"
#include"../../input/IInputManager.h"
#include"GraphInstruction.h"
#include"NewParentInstruction.h"
#include"ChildIndexInstruction.h"

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