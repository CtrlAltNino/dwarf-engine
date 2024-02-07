#pragma once

#include "Core/Base.h"
#include "Editor/EditorModel.h"
#include "Editor/Modules/GuiModule.h"
#include "Editor/Modules/Scene Hierarchy/GraphInstruction.h"

namespace Dwarf
{

    /// @brief Module to display and manipulate the scene graph.
    class SceneHierarchyWindow : public GuiModule
    {
    private:
        /// @brief List of graph instruction. Used as a buffer, executed at the end of a frame.
        std::vector<Ref<GraphInstruction>> m_Instructions;

        std::vector<Entity> m_CopyBuffer;

        /// @brief Draws a graph node for an entity.
        /// @param entity The entity to draw the node of.
        void DrawNode(entt::entity entity);

        /// @brief Executes all buffered instructions.
        void ProcessInstructions();

        void SelectEntity(Entity entity);

        void AddEntityToSelection(Entity entity);

        /// @brief Removes an entity from the selection.
        /// @param entity Entity instance.
        void RemoveEntityFromSelection(Entity entity);

        /// @brief Checks if an entity is selected.
        /// @param entity Entity instance.
        /// @return True if selected, false if not.
        bool IsEntitySelected(Entity entity);

        /// @brief Clears the selection list.
        void ClearSelection();

        std::string GetTreeIndex(Entity entity);

    public:
        SceneHierarchyWindow(Ref<EditorModel> model, int index);

        /// @brief Renders the module window.
        void OnImGuiRender() override;

        void OnUpdate(double deltaTime) override;

        virtual std::string Serialize() override;
    };
}