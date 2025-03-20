#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/IEditor.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "Editor/Modules/IGuiModule.h"
#include "Editor/Modules/SceneHierarchy/GraphInstruction.h"
#include "Editor/Selection/IEditorSelection.h"
#include "Input/IInputManager.h"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  /// @brief Module to display and manipulate the scene graph.
  class SceneHierarchyWindow : public IGuiModule
  {
  private:
    std::shared_ptr<ILoadedScene>     mLoadedScene;
    std::shared_ptr<IEditorSelection> mEditorSelection;
    std::shared_ptr<IInputManager>    mInputManager;
    std::shared_ptr<IAssetDatabase>   mAssetDatabase;
    /// @brief List of graph instruction. Used as a buffer, executed at the end
    /// of a frame.
    std::vector<std::shared_ptr<GraphInstruction>> mInstructions;

    std::vector<entt::entity> mCopyBuffer;

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