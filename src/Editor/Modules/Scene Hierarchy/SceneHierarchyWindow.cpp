#include "dpch.h"

#include <ranges>

#include "Editor/Modules/Scene Hierarchy/SceneHierarchyWindow.h"

#include "Core/Scene/Scene.h"
#include "Editor/Modules/Scene Hierarchy/NewParentInstruction.h"
#include "Editor/Modules/Scene Hierarchy/ChildIndexInstruction.h"
#include "Editor/Modules/Scene Hierarchy/DeleteEntityInstruction.h"
#include "Input/InputManager.h"
#include "Core/Asset/AssetDatabase.h"

namespace Dwarf
{

    SceneHierarchyWindow::SceneHierarchyWindow(Ref<EditorModel> model, int index)
        : GuiModule(model, "Scene Hierarchy", MODULE_TYPE::SCENE_GRAPH, index) {}

    void SceneHierarchyWindow::DrawNode(entt::entity entity)
    {
        // Building the Entity
        Entity ent(entity, m_Model->GetScene()->GetRegistry());
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        std::string objectLabel = ent.GetComponent<NameComponent>().Name;
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        Ref<Scene> scene = m_Model->GetScene();

        // Splitting the channel to draw an extra rect when selected
        draw_list->ChannelsSplit(2);
        draw_list->ChannelsSetCurrent(1);

        // Bool do indicate if the node, which has children, is folded out
        bool opened = false;

        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, IM_COL32(0, 0, 0, 0));
        // Drawing the node, depending on if it has children or not
        if (!ent.GetComponent<TransformComponent>().children.empty())
        {
            opened = ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)ent.GetHandle(), flags, "%s", objectLabel.c_str());
        }
        else
        {
            ImGui::Selectable(std::format("       {}##{}", objectLabel, std::to_string((uint64_t)(uint32_t)ent.GetHandle())).c_str());
        }

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right))
        {
            if (m_Model->GetSelection().IsEntitySelected(ent))
            {
                // Set to selected object
                if (InputManager::GetKey(KEYCODE::LEFT_CONTROL) && ImGui::IsItemClicked(ImGuiMouseButton_Left))
                {
                    m_Model->GetSelection().RemoveEntityFromSelection(ent);
                }
            }
            else
            {
                // Set to selected object
                if (InputManager::GetKey(KEYCODE::LEFT_CONTROL))
                {
                    m_Model->GetSelection().AddEntityToSelection(ent);
                }
                else if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || m_Model->GetSelection().GetSelectedEntities().size() < 2)
                {
                    m_Model->GetSelection().SelectEntity(ent);
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
                    m_Model->GetSelection().SelectEntity(newMesh);
                }

                if (ImGui::MenuItem("Light"))
                {
                    Entity newLight = m_Model->GetScene()->CreateEntity("New Light");
                    newLight.AddComponent<LightComponent>();
                    ent.AddChild(newLight.GetHandle());
                    m_Model->GetSelection().SelectEntity(newLight);
                }

                if (ImGui::MenuItem("Group"))
                {
                    Entity newGroup = m_Model->GetScene()->CreateEntity("New Group");
                    ent.AddChild(newGroup.GetHandle());
                    m_Model->GetSelection().SelectEntity(newGroup);
                }

                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Duplicate"))
            {
                // TODO: instant copy mit richtigen child index

                if ((m_Model->GetSelection().GetSelectionType() == CURRENT_SELECTION_TYPE::ENTITY) && !m_Model->GetSelection().GetSelectedEntities().empty())
                {
                    int index = m_Model->GetSelection().GetSelectedEntities()[m_Model->GetSelection().GetSelectedEntities().size() - 1].GetChildIndex() + 1;
                    for (Entity selectedEntity : m_Model->GetSelection().GetSelectedEntities())
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
                m_CopyBuffer = m_Model->GetSelection().GetSelectedEntities();
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
                m_Instructions.push_back(CreateRef<DeleteEntityInstruction>(m_Model->GetScene(), m_Model->GetSelection().GetSelectedEntities()));
                m_Model->GetSelection().ClearEntitySelection();
            }

            ImGui::EndPopup();
        }

        ImGui::PopStyleColor(2);
        // If this node is being dragged
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_AcceptBeforeDelivery))
        {
            if (!m_Model->GetSelection().IsEntitySelected(ent))
            {
                if (InputManager::GetKey(KEYCODE::LEFT_CONTROL))
                {
                    m_Model->GetSelection().AddEntityToSelection(ent);
                }
                else
                {
                    m_Model->GetSelection().SelectEntity(ent);
                }
            }
            // Set payload to carry the index of our item (could be anything)
            ImGui::SetDragDropPayload("SceneGraphEntity", &m_Model->GetSelection().GetSelectedEntities(), sizeof(m_Model->GetSelection().GetSelectedEntities().at(0)) * m_Model->GetSelection().GetSelectedEntities().size());
            ImGui::Text("%s", objectLabel.c_str());
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

                    auto p1 = ImVec2(p_min.x, p_min.y - 5);
                    auto p2 = ImVec2(p_min.x, p_min.y + 5);
                    auto p3 = ImVec2(p_min.x + 10, p_min.y);

                    auto rectMin = ImVec2(p_min.x, p_min.y - 1);
                    auto rectMax = ImVec2(p_max.x, p_min.y + 1);
                    ImGui::GetForegroundDrawList()->AddTriangleFilled(p1, p2, p3, IM_COL32(76, 86, 106, 255));
                    ImGui::GetWindowDrawList()->AddRectFilled(rectMin, rectMax, IM_COL32(76, 86, 106, 255), 10);
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

            if (const ImGuiPayload *acceptedPayload = ImGui::AcceptDragDropPayload("SceneGraphEntity"))
            {
                IM_ASSERT(acceptedPayload->DataSize == (sizeof(m_Model->GetSelection().GetSelectedEntities().at(0)) * m_Model->GetSelection().GetSelectedEntities().size()));

                std::vector<Entity> payload_e = *(const std::vector<Entity> *)acceptedPayload->Data;

                if (heightFrac < 0.33f)
                {
                    m_Instructions.push_back(CreateRef<NewParentInstruction>(NewParentInstruction(m_Model->GetScene(), payload_e, ent.GetComponent<TransformComponent>().parent)));
                    m_Instructions.push_back(CreateRef<ChildIndexInstruction>(ChildIndexInstruction(m_Model->GetScene(), payload_e, ent.GetChildIndex())));
                }
                else if (heightFrac > 0.66f)
                {
                    m_Instructions.push_back(CreateRef<NewParentInstruction>(NewParentInstruction(m_Model->GetScene(), payload_e, ent.GetComponent<TransformComponent>().parent)));
                    m_Instructions.push_back(CreateRef<ChildIndexInstruction>(ChildIndexInstruction(m_Model->GetScene(), payload_e, ent.GetChildIndex() + 1)));
                }
                else
                {
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
        else if (m_Model->GetSelection().IsEntitySelected(ent))
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

        window_flags |= ImGuiWindowFlags_NoCollapse;

        if (!ImGui::Begin(GetIdentifier().c_str(), &m_WindowOpened, window_flags))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }
        ImGui::PushStyleColor(ImGuiCol_DragDropTarget, ImVec4(0, 0, 0, 0));

        // auto view = m_Model->GetScene()->GetRegistry()->view<TransformComponent, NameComponent, IDComponent>();
        Ref<Scene> scene = m_Model->GetScene();
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
            m_Model->GetSelection().ClearEntitySelection();
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
                    Entity newMesh = m_Model->GetScene()->CreateEntity("New Mesh");
                    newMesh.AddComponent<MeshRendererComponent>();
                    m_Model->GetSelection().SelectEntity(newMesh);
                }

                if (ImGui::MenuItem("Light"))
                {
                    Entity newLight = m_Model->GetScene()->CreateEntity("New Light");
                    newLight.AddComponent<LightComponent>();
                    m_Model->GetSelection().SelectEntity(newLight);
                }

                if (ImGui::MenuItem("Group"))
                {
                    Entity newGroup = m_Model->GetScene()->CreateEntity("New Group");
                    m_Model->GetSelection().SelectEntity(newGroup);
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
                IM_ASSERT(payload->DataSize == (sizeof(m_Model->GetSelection().GetSelectedEntities().at(0)) * m_Model->GetSelection().GetSelectedEntities().size()));

                std::vector<Entity> payload_e = *(const std::vector<Entity> *)payload->Data;
                m_Instructions.push_back(CreateRef<NewParentInstruction>(NewParentInstruction(m_Model->GetScene(), payload_e, m_Model->GetScene()->GetRootEntity()->GetHandle())));
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::PopStyleColor();
        ImGui::End();

        ProcessInstructions();
    }

    void SceneHierarchyWindow::OnUpdate(double deltaTime)
    {
        // Run update code before render
    }

    void SceneHierarchyWindow::Deserialize(nlohmann::json moduleData)
    {
        // Deserialize saved data
    }

    std::string SceneHierarchyWindow::Serialize()
    {
        return "";
    }
}