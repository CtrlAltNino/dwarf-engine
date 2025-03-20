#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Scene/Entity/Entity.h"
#include "pch.h"

#include "Editor/Modules/SceneHierarchy/ChildIndexInstruction.h"
#include "Editor/Modules/SceneHierarchy/DeleteEntityInstruction.h"
#include "Editor/Modules/SceneHierarchy/NewParentInstruction.h"
#include "Editor/Modules/SceneHierarchy/SceneHierarchyWindow.h"

namespace Dwarf
{

  SceneHierarchyWindow::SceneHierarchyWindow(
    std::shared_ptr<ILoadedScene>     loadedScene,
    std::shared_ptr<IEditorSelection> editorSelection,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IAssetDatabase>   assetDatabase)
    : IGuiModule(ModuleLabel("Scene Hierarchy"),
                 ModuleType(MODULE_TYPE::SCENE_GRAPH),
                 ModuleID(std::make_shared<UUID>()))
    , mLoadedScene(loadedScene)
    , mEditorSelection(editorSelection)
    , mInputManager(inputManager)
    , mAssetDatabase(assetDatabase)
  {
  }

  SceneHierarchyWindow::SceneHierarchyWindow(
    std::shared_ptr<ILoadedScene>     loadedScene,
    std::shared_ptr<IEditorSelection> editorSelection,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    SerializedModule                  serializedModule)
    : IGuiModule(ModuleLabel("SceneHierarchy"),
                 ModuleType(MODULE_TYPE::SCENE_GRAPH),
                 ModuleID(std::make_shared<UUID>(
                   serializedModule.t["id"].get<std::string>())))
    , mLoadedScene(loadedScene)
    , mEditorSelection(editorSelection)
    , mInputManager(inputManager)
    , mAssetDatabase(assetDatabase)
  {
    Deserialize(serializedModule.t);
  }

  void
  SceneHierarchyWindow::DrawNode(entt::entity entity)
  {
    // Building the Entity
    Entity             ent(entity, mLoadedScene->GetScene().GetRegistry());
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    std::string        objectLabel = ent.GetComponent<NameComponent>().Name;
    ImDrawList*        draw_list = ImGui::GetWindowDrawList();
    IScene&            scene = mLoadedScene->GetScene();

    // Splitting the channel to draw an extra rect when selected
    draw_list->ChannelsSplit(2);
    draw_list->ChannelsSetCurrent(1);

    // Bool do indicate if the node, which has children, is folded out
    bool opened = false;

    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, IM_COL32(0, 0, 0, 0));
    // Drawing the node, depending on if it has children or not
    if (!ent.GetComponent<TransformComponent>().GetChildren().empty())
    {
      opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)ent.GetHandle(),
                                 flags,
                                 "%s",
                                 objectLabel.c_str());
    }
    else
    {
      ImGui::Selectable(
        std::format("       {}##{}",
                    objectLabel,
                    std::to_string((uint64_t)(uint32_t)ent.GetHandle()))
          .c_str());
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left) ||
        ImGui::IsItemClicked(ImGuiMouseButton_Right))
    {
      if (mEditorSelection->IsEntitySelected(entity))
      {
        // Set to selected object
        if (mInputManager->GetKey(KEYCODE::LEFT_CONTROL) &&
            ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
          mEditorSelection->RemoveEntityFromSelection(entity);
        }
      }
      else
      {
        // Set to selected object
        if (mInputManager->GetKey(KEYCODE::LEFT_CONTROL))
        {
          mEditorSelection->AddEntityToSelection(entity);
        }
        else if (ImGui::IsItemClicked(ImGuiMouseButton_Left) ||
                 mEditorSelection->GetSelectedEntities().size() < 2)
        {
          mEditorSelection->SelectEntity(entity);
        }
      }
    }

    if (ImGui::BeginPopupContextItem())
    {
      if (ImGui::BeginMenu("New"))
      {
        if (ImGui::MenuItem("Mesh"))
        {
          Entity newMesh = scene.CreateEntity("New Mesh");
          newMesh.AddComponent<MeshRendererComponent>();
          ent.AddChild(newMesh.GetHandle());
          mEditorSelection->SelectEntity(newMesh.GetHandle());
        }

        if (ImGui::MenuItem("Light"))
        {
          Entity newLight = scene.CreateEntity("New Light");
          newLight.AddComponent<LightComponent>();
          ent.AddChild(newLight.GetHandle());
          mEditorSelection->SelectEntity(newLight.GetHandle());
        }

        if (ImGui::MenuItem("Group"))
        {
          Entity newGroup = scene.CreateEntity("New Group");
          ent.AddChild(newGroup.GetHandle());
          mEditorSelection->SelectEntity(newGroup.GetHandle());
        }

        ImGui::EndMenu();
      }

      if (ImGui::MenuItem("Duplicate"))
      {
        // TODO: instant copy mit richtigen child index

        if ((mEditorSelection->GetSelectionType() ==
             CURRENT_SELECTION_TYPE::ENTITY) &&
            !mEditorSelection->GetSelectedEntities().empty())
        {
          Entity entity(mEditorSelection->GetSelectedEntities().at(
                          mEditorSelection->GetSelectedEntities().size() - 1),
                        scene.GetRegistry());
          int    index = entity.GetChildIndex() + 1;
          for (entt::entity selectedEntity :
               mEditorSelection->GetSelectedEntities())
          {
            Entity source = Entity(selectedEntity, scene.GetRegistry());
            Entity copy = scene.CreateEntity(
              source.GetComponent<NameComponent>().Name + " Copy");
            copy.SetParent(source.GetParent());
            copy.SetChildIndex(index++);

            if (source.HasComponent<LightComponent>())
            {
              copy.AddComponent<LightComponent>(
                source.GetComponent<LightComponent>());
            }

            if (source.HasComponent<MeshRendererComponent>())
            {
              std::unique_ptr<IAssetReference> copiedMesh =
                source.GetComponent<MeshRendererComponent>().GetModelAsset() !=
                    nullptr
                  ? mAssetDatabase->Retrieve(source.GetUID())
                  : nullptr;

              std::map<int, std::unique_ptr<IAssetReference>> copiedMaterials;

              for (auto& material : source.GetComponent<MeshRendererComponent>()
                                      .GetMaterialAssets())
              {
                copiedMaterials.at(material.first) =
                  mAssetDatabase->Retrieve(material.second->GetUID());
              }

              copy.AddComponent<MeshRendererComponent>(MeshRendererComponent(
                std::move(copiedMesh), std::move(copiedMaterials)));
            }
          }
        }
      }

      if (ImGui::MenuItem("Copy"))
      {
        mCopyBuffer =
          std::vector<entt::entity>(mEditorSelection->GetSelectedEntities());
      }

      if (ImGui::MenuItem("Paste"))
      {
        // TODO: Implement pasting of deleted entities
        for (entt::entity selectedEntity : mCopyBuffer)
        {
          Entity source = Entity(selectedEntity, scene.GetRegistry());
          Entity copy = scene.CreateEntity(
            source.GetComponent<NameComponent>().Name + " Copy");
          copy.SetParent(entity);

          if (source.HasComponent<LightComponent>())
          {
            copy.AddComponent<LightComponent>(
              source.GetComponent<LightComponent>());
          }

          if (source.HasComponent<MeshRendererComponent>())
          {
            std::unique_ptr<IAssetReference> copiedMesh =
              source.GetComponent<MeshRendererComponent>().GetModelAsset() !=
                  nullptr
                ? mAssetDatabase->Retrieve(source.GetUID())
                : nullptr;

            std::map<int, std::unique_ptr<IAssetReference>> copiedMaterials;

            for (auto& material : source.GetComponent<MeshRendererComponent>()
                                    .GetMaterialAssets())
            {
              copiedMaterials.at(material.first) =
                mAssetDatabase->Retrieve(material.second->GetUID());
            }

            copy.AddComponent<MeshRendererComponent>(
              std::move(copiedMesh), std::move(copiedMaterials));
          }
        }
      }

      if (ImGui::MenuItem("Delete"))
      {
        mInstructions.push_back(std::make_shared<DeleteEntityInstruction>(
          mLoadedScene->GetScene(), mEditorSelection->GetSelectedEntities()));
        mEditorSelection->ClearEntitySelection();
      }

      ImGui::EndPopup();
    }

    ImGui::PopStyleColor(2);
    // If this node is being dragged
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_AcceptBeforeDelivery))
    {
      if (!mEditorSelection->IsEntitySelected(entity))
      {
        if (mInputManager->GetKey(KEYCODE::LEFT_CONTROL))
        {
          mEditorSelection->AddEntityToSelection(entity);
        }
        else
        {
          mEditorSelection->SelectEntity(entity);
        }
      }
      // Set payload to carry the index of our item (could be anything)
      ImGui::SetDragDropPayload(
        "SceneGraphEntity",
        &mEditorSelection->GetSelectedEntities(),
        sizeof(mEditorSelection->GetSelectedEntities().at(0)) *
          mEditorSelection->GetSelectedEntities().size());
      ImGui::Text("%s", objectLabel.c_str());
      ImGui::EndDragDropSource();
    }

    // If something is dropped on this node
    ImGui::PushStyleColor(ImGuiCol_DragDropTarget, ImVec4(0, 0, 0, 0));

    if (ImGui::BeginDragDropTarget())
    {
      float               heightFrac = -1;
      const ImGuiPayload* payload = ImGui::GetDragDropPayload();
      if (payload->IsDataType("SceneGraphEntity"))
      {
        heightFrac = (ImGui::GetMousePos().y - ImGui::GetItemRectMin().y) /
                     (ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y);

        if (heightFrac < 0.33f)
        {
          ImVec2 p_min = ImGui::GetItemRectMin();
          ImVec2 p_max = ImGui::GetItemRectMax();

          auto p1 = ImVec2(p_min.x, p_min.y - 5);
          auto p2 = ImVec2(p_min.x, p_min.y + 5);
          auto p3 = ImVec2(p_min.x + 10, p_min.y);

          auto rectMin = ImVec2(p_min.x, p_min.y - 1);
          auto rectMax = ImVec2(p_max.x, p_min.y + 1);
          ImGui::GetForegroundDrawList()->AddTriangleFilled(
            p1, p2, p3, IM_COL32(76, 86, 106, 255));
          ImGui::GetWindowDrawList()->AddRectFilled(
            rectMin, rectMax, IM_COL32(76, 86, 106, 255), 10);
        }
        else if (heightFrac > 0.66f)
        {
          ImVec2 p_min = ImGui::GetItemRectMin();
          ImVec2 p_max = ImGui::GetItemRectMax();

          auto p1 = ImVec2(p_min.x, p_max.y - 5);
          auto p2 = ImVec2(p_min.x, p_max.y + 5);
          auto p3 = ImVec2(p_min.x + 10, p_max.y);

          auto rectMin = ImVec2(p_min.x, p_max.y - 1);
          auto rectMax = ImVec2(p_max.x, p_max.y + 1);
          ImGui::GetForegroundDrawList()->AddTriangleFilled(
            p1, p2, p3, IM_COL32(76, 86, 106, 255));
          ImGui::GetWindowDrawList()->AddRectFilled(
            rectMin, rectMax, IM_COL32(76, 86, 106, 255), 10);
        }
        else
        {
          draw_list->ChannelsSetCurrent(0);
          ImVec2 p_min = ImGui::GetItemRectMin();
          ImVec2 p_max = ImGui::GetItemRectMax();
          ImU32  rectCol = IM_COL32(76, 86, 106, 255);
          ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, rectCol, 10);
        }
      }

      if (const ImGuiPayload* acceptedPayload =
            ImGui::AcceptDragDropPayload("SceneGraphEntity"))
      {
        IM_ASSERT(acceptedPayload->DataSize ==
                  (sizeof(mEditorSelection->GetSelectedEntities().at(0)) *
                   mEditorSelection->GetSelectedEntities().size()));

        std::vector<entt::entity> payload_e =
          *(std::vector<entt::entity>*)acceptedPayload->Data;

        if (heightFrac < 0.33f)
        {
          mInstructions.push_back(std::make_shared<NewParentInstruction>(
            mLoadedScene->GetScene(),
            payload_e,
            ent.GetComponent<TransformComponent>().GetParent()));
          mInstructions.push_back(std::make_shared<ChildIndexInstruction>(
            mLoadedScene->GetScene(), payload_e, ent.GetChildIndex()));
        }
        else if (heightFrac > 0.66f)
        {
          mInstructions.push_back(std::make_shared<NewParentInstruction>(
            mLoadedScene->GetScene(),
            payload_e,
            ent.GetComponent<TransformComponent>().GetParent()));
          mInstructions.push_back(std::make_shared<ChildIndexInstruction>(
            mLoadedScene->GetScene(), payload_e, ent.GetChildIndex() + 1));
        }
        else
        {
          mInstructions.push_back(std::make_shared<NewParentInstruction>(
            mLoadedScene->GetScene(), payload_e, entity));
        }
      }
      ImGui::EndDragDropTarget();
    }

    if (ImGui::IsItemHovered())
    {
      draw_list->ChannelsSetCurrent(0);
      ImU32 rectCol = IM_COL32(76, 86, 106, 255);
      if (ImGui::IsMouseDown(0))
      {
        rectCol = IM_COL32(86, 95, 114, 255);
      }
      draw_list->AddRectFilled(
        ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), rectCol, 5.0f);
    }
    else if (mEditorSelection->IsEntitySelected(entity))
    {
      draw_list->ChannelsSetCurrent(0);
      draw_list->AddRectFilled(ImGui::GetItemRectMin(),
                               ImGui::GetItemRectMax(),
                               IM_COL32(76, 86, 106, 255),
                               5.0f);
    }

    ImGui::PopStyleColor();

    // Merge channels
    draw_list->ChannelsMerge();
    // Draw the child nodes if the node has children and is folded out
    if (opened)
    {
      for (entt::entity child :
           ent.GetComponent<TransformComponent>().GetChildren())
      {
        DrawNode(child);
      }
      ImGui::TreePop();
    }
  }

  void
  SceneHierarchyWindow::ProcessInstructions()
  {
    for (std::shared_ptr<GraphInstruction> instruction : mInstructions)
    {
      instruction->PerformInstruction();
    }

    mInstructions.clear();
  }

  void
  SceneHierarchyWindow::OnImGuiRender()
  {
    ImGuiWindowFlags window_flags = 0;

    window_flags |= ImGuiWindowFlags_NoCollapse;

    if (!ImGui::Begin(GetIdentifier().c_str(), &mWindowOpened, window_flags))
    {
      // Early out if the window is collapsed, as an optimization.
      ImGui::End();
      return;
    }
    ImGui::PushStyleColor(ImGuiCol_DragDropTarget, ImVec4(0, 0, 0, 0));

    std::vector<entt::entity>& container = mLoadedScene->GetScene()
                                             .GetRootEntity()
                                             .GetComponent<TransformComponent>()
                                             .GetChildren();

    int it = 0;
    while ((container.begin() + it) != container.end())
    {
      DrawNode(*(container.begin() + it));
      it++;
    }

    ImGui::Dummy(ImGui::GetContentRegionAvail());

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
      mEditorSelection->ClearEntitySelection();
    }
    else if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
    {
      ImGui::OpenPopup("RightClickMenuEmpty");
    }

    if (ImGui::BeginPopupContextItem("RightClickMenuEmpty"))
    {
      if (ImGui::BeginMenu("New"))
      {
        if (ImGui::MenuItem("Mesh"))
        {
          Entity newMesh = mLoadedScene->GetScene().CreateEntity("New Mesh");
          newMesh.AddComponent<MeshRendererComponent>();
          mEditorSelection->SelectEntity(newMesh.GetHandle());
        }

        if (ImGui::MenuItem("Light"))
        {
          Entity newLight = mLoadedScene->GetScene().CreateEntity("New Light");
          newLight.AddComponent<LightComponent>();
          mEditorSelection->SelectEntity(newLight.GetHandle());
        }

        if (ImGui::MenuItem("Group"))
        {
          Entity newGroup = mLoadedScene->GetScene().CreateEntity("New Group");
          mEditorSelection->SelectEntity(newGroup.GetHandle());
        }
        ImGui::EndPopup();
      }

      if (ImGui::MenuItem("Paste"))
      {
        // TODO: Implement pasting deleted entities
        Entity& rootEntity = mLoadedScene->GetScene().GetRootEntity();
        for (entt::entity selectedEntity : mCopyBuffer)
        {
          Entity source =
            Entity(selectedEntity, mLoadedScene->GetScene().GetRegistry());
          Entity copy = mLoadedScene->GetScene().CreateEntity(
            source.GetComponent<NameComponent>().Name + " Copy");

          if (source.HasComponent<LightComponent>())
          {
            copy.AddComponent<LightComponent>(
              source.GetComponent<LightComponent>());
          }

          if (source.HasComponent<MeshRendererComponent>())
          {
            std::unique_ptr<IAssetReference> copiedMesh =
              source.GetComponent<MeshRendererComponent>().GetModelAsset() !=
                  nullptr
                ? mAssetDatabase->Retrieve(source.GetUID())
                : nullptr;

            std::map<int, std::unique_ptr<IAssetReference>> copiedMaterials;

            for (auto& material : source.GetComponent<MeshRendererComponent>()
                                    .GetMaterialAssets())
            {
              copiedMaterials.at(material.first) =
                mAssetDatabase->Retrieve(material.second->GetUID());
            }

            copy.AddComponent<MeshRendererComponent>(
              std::move(copiedMesh), std::move(copiedMaterials));
          }
        }
      }

      ImGui::EndPopup();
    }

    // If something is dropped on this node
    if (ImGui::BeginDragDropTarget())
    {
      if (const ImGuiPayload* payload =
            ImGui::AcceptDragDropPayload("SceneGraphEntity"))
      {
        IM_ASSERT(payload->DataSize ==
                  (sizeof(mEditorSelection->GetSelectedEntities().at(0)) *
                   mEditorSelection->GetSelectedEntities().size()));

        std::vector<entt::entity> payload_e =
          *(const std::vector<entt::entity>*)payload->Data;
        mInstructions.push_back(std::make_shared<NewParentInstruction>(
          mLoadedScene->GetScene(),
          payload_e,
          mLoadedScene->GetScene().GetRootEntity().GetHandle()));
      }
      ImGui::EndDragDropTarget();
    }

    ImGui::PopStyleColor();
    ImGui::End();

    ProcessInstructions();
  }

  void
  SceneHierarchyWindow::OnUpdate()
  {
    // Run update code before render
  }

  void
  SceneHierarchyWindow::Deserialize(nlohmann::json moduleData)
  {
    // Deserialize saved data
  }

  nlohmann::json
  SceneHierarchyWindow::Serialize()
  {
    nlohmann::json serializedModule;

    serializedModule["id"] = GetUuid()->toString();
    serializedModule["type"] = static_cast<int>(GetModuleType());
    serializedModule["label"] = GetModuleName();

    return serializedModule;
  }
}