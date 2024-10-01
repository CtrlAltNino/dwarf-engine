#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/IEditor.h"
#include "Editor/Selection/IEditorSelection.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "Editor/Modules/IGuiModule.h"
#include "Editor/Modules/SceneHierarchy/GraphInstruction.h"
#include "Input/IInputManager.h"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  /// @brief Module to display and manipulate the scene graph.
  class SceneHierarchyWindow : public IGuiModule
  {
  private:
    std::shared_ptr<ILoadedScene>     m_LoadedScene;
    std::shared_ptr<IEditorSelection> m_EditorSelection;
    std::shared_ptr<IInputManager>    m_InputManager;
    std::shared_ptr<IAssetDatabase>   m_AssetDatabase;
    /// @brief List of graph instruction. Used as a buffer, executed at the end
    /// of a frame.
    std::vector<std::shared_ptr<GraphInstruction>> m_Instructions;

    std::vector<Entity> m_CopyBuffer;

    /// @brief Draws a graph node for an entity.
    /// @param entity The entity to draw the node of.
    void
    DrawNode(entt::entity entity);

    /// @brief Executes all buffered instructions.
    void
    ProcessInstructions();

  public:
    SceneHierarchyWindow(std::shared_ptr<ILoadedScene>     loadedScene,
                         std::shared_ptr<IEditorSelection> editorSelection,
                         std::shared_ptr<IInputManager>    inputManager,
                         std::shared_ptr<IAssetDatabase>   assetDatabase);

    SceneHierarchyWindow(std::shared_ptr<ILoadedScene>     loadedScene,
                         std::shared_ptr<IEditorSelection> editorSelection,
                         std::shared_ptr<IInputManager>    inputManager,
                         std::shared_ptr<IAssetDatabase>   assetDatabase,
                         SerializedModule                  serializedModule);

    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    void
    OnUpdate() override;

    nlohmann::json
    Serialize() override;

    void
    Deserialize(nlohmann::json moduleData);
  };
}