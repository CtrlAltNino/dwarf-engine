#include "dpch.h"

#include "Editor/Modules/Scene Hierarchy/SceneHierarchyWindow.h"

#include "Core/Scene/Scene.h"
#include "Editor/Modules/Scene Hierarchy/NewParentInstruction.h"
#include "Editor/Modules/Scene Hierarchy/ChildIndexInstruction.h"
#include "Editor/Modules/Scene Hierarchy/DeleteEntityInstruction.h"
#include "Input/InputManager.h"

namespace Dwarf
{

    SceneHierarchyWindow::SceneHierarchyWindow(Ref<EditorModel> model, int index)
        : GuiModule(model, "Scene Hierarchy", MODULE_TYPE::SCENE_GRAPH, index) {}

    void SceneHierarchyWindow::DrawNode(entt::entity entity)
    {
        // Building the Entity
        Entity ent = Entity(entity, m_Model->GetScene()->GetRegistry());
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        std::string objectLabel = ent.GetComponent<NameComponent>().Name;
        ImDrawList *draw_list = ImGui::GetWindowDrawList();

        // Splitting the channel to draw an extra rect when selected
        draw_list->ChannelsSplit(2);
        draw_list->ChannelsSetCurrent(1);

        // Bool do indicate if the node, which has children, is folded out
        bool opened = false;

        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, IM_COL32(0, 0, 0, 0));
        // Drawing the node, depending on if it has children or not
        if (ent.GetComponent<TransformComponent>().children.size() != 0)
        {
            opened = ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)ent.GetHandle(), flags, objectLabel.c_str());
        }
        else
        {
            ImGui::Selectable(("       " + objectLabel + "##" + std::to_string((uint64_t)(uint32_t)ent.GetHandle())).c_str());
        }

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right))
        {
            if (IsEntitySelected(ent))
            {
                // Set to selected object
                if (InputManager::GetKey(KEYCODE::LEFT_CONTROL) && ImGui::IsItemClicked(ImGuiMouseButton_Left))
                {
                    RemoveEntityFromSelection(ent);
                }
            }
            else
            {
                // Set to selected object
                if (InputManager::GetKey(KEYCODE::LEFT_CONTROL))
                {
                    AddEntityToSelection(ent);
                }
                else if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || m_Model->m_Selection.selectedEntities.size() < 2)
                {
                    SelectEntity(ent);
                }
            }
        }

        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::BeginMenu("New"))
            {
                if (ImGui::MenuItem("Mesh"))
                {
                    Entity newMesh = m_Model->GetScene()->CreateEntity("New Mesh");
                    newMesh.AddComponent<MeshRendererComponent>();
                    ent.AddChild(newMesh.GetHandle());
                    SelectEntity(newMesh);
                }

                if (ImGui::MenuItem("Light"))
                {
                    Entity newLight = m_Model->GetScene()->CreateEntity("New Light");
                    newLight.AddComponent<LightComponent>();
                    ent.AddChild(newLight.GetHandle());
                    SelectEntity(newLight);
                }

                if (ImGui::MenuItem("Group"))
                {
                    Entity newGroup = m_Model->GetScene()->CreateEntity("New Group");
                    ent.AddChild(newGroup.GetHandle());
                    SelectEntity(newGroup);
                }

                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Duplicate"))
            {
                // TODO: instant copy mit richtigen child index

                if ((m_Model->m_Selection.selectionType == INSPECTOR_SELECTION_TYPE::ENTITY) && (m_Model->m_Selection.selectedEntities.size() > 0))
                {
                    int index = m_Model->m_Selection.selectedEntities[m_Model->m_Selection.selectedEntities.size() - 1].GetChildIndex() + 1;
                    for (Entity selectedEntity : m_Model->m_Selection.selectedEntities)
                    {
                        Entity copy = m_Model->GetScene()->CreateEntity(selectedEntity.GetComponent<NameComponent>().Name + " Copy");
                        copy.SetParent(selectedEntity.GetParent());
                        copy.SetChildIndex(index++);

                        if (selectedEntity.HasComponent<LightComponent>())
                        {
                            copy.AddComponent<LightComponent>(selectedEntity.GetComponent<LightComponent>());
                        }

                        if (selectedEntity.HasComponent<MeshRendererComponent>())
                        {
                            copy.AddComponent<MeshRendererComponent>(selectedEntity.GetComponent<MeshRendererComponent>());
                        }
                    }
                }
            }

            if (ImGui::MenuItem("Copy"))
            {
                m_CopyBuffer = m_Model->m_Selection.selectedEntities;
            }

            if (ImGui::MenuItem("Paste"))
            {
                // TODO: Implement pasting of deleted entities
                for (Entity selectedEntity : m_CopyBuffer)
                {
                    Entity copy = m_Model->GetScene()->CreateEntity(selectedEntity.GetComponent<NameComponent>().Name + " Copy");
                    copy.SetParent(entity);

                    if (selectedEntity.HasComponent<LightComponent>())
                    {
                        copy.AddComponent<LightComponent>(selectedEntity.GetComponent<LightComponent>());
                    }

                    if (selectedEntity.HasComponent<MeshRendererComponent>())
                    {
                        copy.AddComponent<MeshRendererComponent>(selectedEntity.GetComponent<MeshRendererComponent>());
                    }
                }
            }

            if (ImGui::MenuItem("Delete"))
            {
                std::cout << "Deleting" << std::endl;
                m_Instructions.push_back(CreateRef<DeleteEntityInstruction>(m_Model->GetScene(), m_Model->m_Selection.selectedEntities));
                m_Model->m_Selection.selectionType = INSPECTOR_SELECTION_TYPE::NONE;
                m_Model->m_Selection.selectedEntities.clear();
            }

            ImGui::EndPopup();
        }

        ImGui::PopStyleColor(2);
        // If this node is being dragged
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_AcceptBeforeDelivery))
        {
            if (!IsEntitySelected(ent))
            {
                // if(model->GetInput()->GetKey(KEYCODE::LEFT_CONTROL)){
                if (InputManager::GetKey(KEYCODE::LEFT_CONTROL))
                {
                    AddEntityToSelection(ent);
                }
                else
                {
                    SelectEntity(ent);
                }
            }
            // Set payload to carry the index of our item (could be anything)
            ImGui::SetDragDropPayload("SceneGraphEntity", &m_Model->m_Selection.selectedEntities, sizeof(m_Model->m_Selection.selectedEntities.at(0)) * m_Model->m_Selection.selectedEntities.size());
            ImGui::Text(objectLabel.c_str());
            ImGui::EndDragDropSource();
        }

        // If something is dropped on this node
        ImGui::PushStyleColor(ImGuiCol_DragDropTarget, ImVec4(0, 0, 0, 0));

        if (ImGui::BeginDragDropTarget())
        {
            float heightFrac = -1;
            const ImGuiPayload *payload = ImGui::GetDragDropPayload();
            if (payload->IsDataType("SceneGraphEntity"))
            {
                heightFrac = (ImGui::GetMousePos().y - ImGui::GetItemRectMin().y) / (ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y);

                if (heightFrac < 0.33f)
                {
                    ImVec2 p_min = ImGui::GetItemRectMin();
                    ImVec2 p_max = ImGui::GetItemRectMax();

                    ImVec2 p1 = ImVec2(p_min.x, p_min.y - 5);
                    ImVec2 p2 = ImVec2(p_min.x, p_min.y + 5);
                    ImVec2 p3 = ImVec2(p_min.x + 10, p_min.y);

                    ImVec2 rectMin = ImVec2(p_min.x, p_min.y - 1);
                    ImVec2 rectMax = ImVec2(p_max.x, p_min.y + 1);
                    ImGui::GetForegroundDrawList()->AddTriangleFilled(p1, p2, p3, IM_COL32(76, 86, 106, 255));
                    ImGui::GetWindowDrawList()->AddRectFilled(rectMin, rectMax, IM_COL32(76, 86, 106, 255), 10);
                }
                else if (heightFrac > 0.66f)
                {
                    ImVec2 p_min = ImGui::GetItemRectMin();
                    ImVec2 p_max = ImGui::GetItemRectMax();

                    ImVec2 p1 = ImVec2(p_min.x, p_max.y - 5);
                    ImVec2 p2 = ImVec2(p_min.x, p_max.y + 5);
                    ImVec2 p3 = ImVec2(p_min.x + 10, p_max.y);

                    ImVec2 rectMin = ImVec2(p_min.x, p_max.y - 1);
                    ImVec2 rectMax = ImVec2(p_max.x, p_max.y + 1);
                    ImGui::GetForegroundDrawList()->AddTriangleFilled(p1, p2, p3, IM_COL32(76, 86, 106, 255));
                    ImGui::GetWindowDrawList()->AddRectFilled(rectMin, rectMax, IM_COL32(76, 86, 106, 255), 10);
                }
                else
                {
                    draw_list->ChannelsSetCurrent(0);
                    ImVec2 p_min = ImGui::GetItemRectMin();
                    ImVec2 p_max = ImGui::GetItemRectMax();
                    ImU32 rectCol = IM_COL32(76, 86, 106, 255);
                    ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, rectCol, 10);
                }
            }

            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("SceneGraphEntity"))
            {
                IM_ASSERT(payload->DataSize == (sizeof(m_Model->m_Selection.selectedEntities.at(0)) * m_Model->m_Selection.selectedEntities.size()));

                std::vector<Entity> payload_e = *(const std::vector<Entity> *)payload->Data;
                std::vector<Entity>::iterator it;

                if (heightFrac < 0.33f)
                {
                    m_Instructions.push_back(CreateRef<NewParentInstruction>(NewParentInstruction(m_Model->GetScene(), payload_e, ent.GetComponent<TransformComponent>().parent)));
                    m_Instructions.push_back(CreateRef<ChildIndexInstruction>(ChildIndexInstruction(m_Model->GetScene(), payload_e, ent.GetChildIndex())));
                }
                else if (heightFrac > 0.66f)
                {
                    // droppedEntity.SetParent(ent.GetComponent<TransformComponent>().parent);
                    // droppedEntity.SetChildIndex(ent.GetChildIndex()+1);
                    m_Instructions.push_back(CreateRef<NewParentInstruction>(NewParentInstruction(m_Model->GetScene(), payload_e, ent.GetComponent<TransformComponent>().parent)));
                    m_Instructions.push_back(CreateRef<ChildIndexInstruction>(ChildIndexInstruction(m_Model->GetScene(), payload_e, ent.GetChildIndex() + 1)));
                }
                else
                {
                    // droppedEntity.SetParent(entity);
                    m_Instructions.push_back(CreateRef<NewParentInstruction>(NewParentInstruction(m_Model->GetScene(), payload_e, entity)));
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
            draw_list->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), rectCol, 5.0f);
        }
        else if (IsEntitySelected(ent))
        {
            draw_list->ChannelsSetCurrent(0);
            draw_list->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(76, 86, 106, 255), 5.0f);
        }

        ImGui::PopStyleColor();

        // Merge channels
        draw_list->ChannelsMerge();
        // Draw the child nodes if the node has children and is folded out
        if (opened)
        {
            for (entt::entity child : ent.GetComponent<TransformComponent>().children)
            {
                DrawNode(child);
            }
            ImGui::TreePop();
        }
    }

    void SceneHierarchyWindow::ProcessInstructions()
    {
        for (Ref<GraphInstruction> instruction : m_Instructions)
        {
            instruction->PerformInstruction();
        }

        m_Instructions.clear();
    }

    void SceneHierarchyWindow::OnImGuiRender()
    {
        ImGuiWindowFlags window_flags = 0;

        // window_flags |= ImGuiWindowFlags_NoMove;
        // window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoCollapse;
        // window_flags |= ImGuiWindowFlags_NoTitleBar;
        // window_flags |= ImGuiWindowFlags_MenuBar;

        if (!ImGui::Begin(GetIdentifier().c_str(), &m_WindowOpened, window_flags))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }
        ImGui::PushStyleColor(ImGuiCol_DragDropTarget, ImVec4(0, 0, 0, 0));

        auto view = m_Model->GetScene()->GetRegistry()->view<TransformComponent, NameComponent, IDComponent>();
        std::vector<entt::entity> *container = &(m_Model->GetScene()->GetRootEntity()->GetComponent<TransformComponent>().children);

        int it = 0;
        while ((container->begin() + it) != container->end())
        {
            DrawNode(*(container->begin() + it));
            it++;
        }

        ImGui::Dummy(ImGui::GetContentRegionAvail());

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            ClearSelection();
        }
        else if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
        {
            ImGui::OpenPopupContextItem("RightClickMenuEmpty");
        }

        if (ImGui::BeginPopupContextItem("RightClickMenuEmpty"))
        {
            if (ImGui::BeginMenu("New"))
            {
                if (ImGui::MenuItem("Mesh"))
                {
                    Entity newMesh = m_Model->GetScene()->CreateEntity("New Mesh");
                    newMesh.AddComponent<MeshRendererComponent>();
                    SelectEntity(newMesh);
                }

                if (ImGui::MenuItem("Light"))
                {
                    Entity newLight = m_Model->GetScene()->CreateEntity("New Light");
                    newLight.AddComponent<LightComponent>();
                    SelectEntity(newLight);
                }

                if (ImGui::MenuItem("Group"))
                {
                    Entity newGroup = m_Model->GetScene()->CreateEntity("New Group");
                    SelectEntity(newGroup);
                }
                ImGui::EndPopup();
            }

            if (ImGui::MenuItem("Paste"))
            {
                // TODO: Implement pasting deleted entities
                Ref<Entity> rootEntity = m_Model->GetScene()->GetRootEntity();
                for (Entity selectedEntity : m_CopyBuffer)
                {
                    Entity copy = m_Model->GetScene()->CreateEntity(selectedEntity.GetComponent<NameComponent>().Name + " Copy");

                    if (selectedEntity.HasComponent<LightComponent>())
                    {
                        copy.AddComponent<LightComponent>(selectedEntity.GetComponent<LightComponent>());
                    }

                    if (selectedEntity.HasComponent<MeshRendererComponent>())
                    {
                        copy.AddComponent<MeshRendererComponent>(selectedEntity.GetComponent<MeshRendererComponent>());
                    }
                }
            }

            ImGui::EndPopup();
        }

        // If something is dropped on this node
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("SceneGraphEntity"))
            {
                IM_ASSERT(payload->DataSize == (sizeof(m_Model->m_Selection.selectedEntities.at(0)) * m_Model->m_Selection.selectedEntities.size()));

                std::vector<Entity> payload_e = *(const std::vector<Entity> *)payload->Data;
                m_Instructions.push_back(CreateRef<NewParentInstruction>(NewParentInstruction(m_Model->GetScene(), payload_e, m_Model->GetScene()->GetRootEntity()->GetHandle())));
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::PopStyleColor();
        ImGui::End();

        ProcessInstructions();
    }

    void SceneHierarchyWindow::SelectEntity(Entity entity)
    {
        ClearSelection();
        AddEntityToSelection(entity);
    }

    void SceneHierarchyWindow::AddEntityToSelection(Entity entity)
    {
        std::string index = GetTreeIndex(entity);

        std::vector<Entity>::iterator cursor = m_Model->m_Selection.selectedEntities.begin();
        std::string cursorIndex;

        while ((cursor != m_Model->m_Selection.selectedEntities.end()) && ((cursorIndex = GetTreeIndex(*cursor)) < index))
        {
            cursor++;
        }

        if (cursor == m_Model->m_Selection.selectedEntities.end())
        {
            m_Model->m_Selection.selectedEntities.push_back(entity);
        }
        else
        {
            m_Model->m_Selection.selectedEntities.insert(cursor, entity);
        }

        m_Model->m_Selection.selectionType = INSPECTOR_SELECTION_TYPE::ENTITY;
    }

    void SceneHierarchyWindow::ClearSelection()
    {
        m_Model->m_Selection.selectedEntities.clear();
        m_Model->m_Selection.selectionType = INSPECTOR_SELECTION_TYPE::NONE;
    }

    void SceneHierarchyWindow::RemoveEntityFromSelection(Entity entity)
    {
        m_Model->m_Selection.selectedEntities.erase(std::find(m_Model->m_Selection.selectedEntities.begin(), m_Model->m_Selection.selectedEntities.end(), entity));
    }

    bool SceneHierarchyWindow::IsEntitySelected(Entity entity)
    {
        return std::find(m_Model->m_Selection.selectedEntities.begin(), m_Model->m_Selection.selectedEntities.end(), entity) != m_Model->m_Selection.selectedEntities.end();
    }

    std::string SceneHierarchyWindow::GetTreeIndex(Entity entity)
    {
        std::string index = "";
        Entity cursor = entity;

        while (cursor.GetHandle() != m_Model->GetScene()->GetRootEntity()->GetHandle())
        {
            index = std::to_string(cursor.GetChildIndex()) + index;
            cursor = Entity(cursor.GetParent(), m_Model->GetScene()->GetRegistry());
        }

        return index;
    }

    void SceneHierarchyWindow::OnUpdate(double deltaTime)
    {
    }

    std::string SceneHierarchyWindow::Serialize()
    {
        return "";
    }
}