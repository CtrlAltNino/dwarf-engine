#pragma once

#include <entt/entt.hpp>
#include <algorithm>

#include "Core/Base.h"
#include "Core/UID.h"
#include "Core/GenericComponents.h"
#include "Core/Scene/SceneComponents.h"

namespace Dwarf
{

  /// @brief Class wrapping around an entity, providing gameobject
  /// functionality.
  class Entity
  {
  public:
    Entity(entt::entity handle, std::shared_ptr<entt::registry> registry);
    Entity(const Entity& other) = default;

    /// @brief Adds a component to the entity.
    /// @tparam T Type of component.
    /// @tparam ...Args Component arguments.
    /// @param ...args Components to add.
    /// @return The added component.
    template<typename T, typename... Args>
    T&
    AddComponent(Args&&... args)
    {
      // TODO: Check component requirements
      return m_Registry->emplace<T>(m_EntityHandle,
                                    std::forward<Args>(args)...);
    }

    /// @brief Retrieves a component of the Entity if present.
    /// @tparam T Type of component.
    /// @return The found component.
    template<typename T>
    T&
    GetComponent()
    {
      // TODO: Check if component present
      return m_Registry->get<T>(m_EntityHandle);
    }

    /// @brief Removes a component from the entity.
    /// @tparam T Type of component to remove.
    template<typename T>
    void
    RemoveComponent()
    {
      // TODO: Check if component is present
      if (HasComponent<T>())
      {
        m_Registry->remove<T>(m_EntityHandle);
      }
    }

    /// @brief Checks if the entity has a component assigned.
    /// @tparam T Type of component to check for.
    /// @return True if component is present, false if not.
    template<typename T>
    bool
    HasComponent()
    {
      return m_Registry->try_get<T>(m_EntityHandle) != nullptr;
    }

    /// @brief Retrieves the UID of the entity.
    /// @return The UID.
    std::shared_ptr<UID>
    GetUID()
    {
      return GetComponent<IDComponent>().ID;
    }

    operator bool() const { return (std::uint32_t)m_EntityHandle != 0; }
    bool
    operator==(const Entity& b)
    {
      return m_EntityHandle == b.m_EntityHandle;
    }

    /// @brief Sets a new parent for this entity.
    /// @param entity The entity handle of the parent-to-be.
    void
    SetParent(entt::entity entity)
    {
      TransformComponent transform = GetComponent<TransformComponent>();
      auto               newParent = Entity(entity, m_Registry);

      if (transform.parent != entt::null)
      {
        auto oldParent = Entity(transform.parent, m_Registry);
        oldParent.RemoveChild(m_EntityHandle);
      }

      GetComponent<TransformComponent>().parent = entity;
      if (m_Registry->valid(entity))
      {
        newParent.AddChild(m_EntityHandle);
      }
    }

    /// @brief Adds a new child entity to this entity.
    /// @param entity Entity handle of the new child.
    void
    AddChild(entt::entity entity)
    {
      TransformComponent transform = GetComponent<TransformComponent>();

      if (transform.children.empty() ||
          (std::ranges::find(transform.children.begin(),
                             transform.children.end(),
                             entity) == transform.children.end()))
      {
        GetComponent<TransformComponent>().children.push_back(entity);
      }
    }

    /// @brief Removes a child from the list of children.
    /// @param entity Entity handle of the entity to remove.
    void
    RemoveChild(entt::entity entity)
    {
      auto it =
        std::ranges::find(GetComponent<TransformComponent>().children.begin(),
                          GetComponent<TransformComponent>().children.end(),
                          entity);
      if (it != GetComponent<TransformComponent>().children.end())
      {
        GetComponent<TransformComponent>().children.erase(it);
      }
    }

    /// @brief Sets the position this entity should take between its siblings.
    /// @param index Index for the list of childrens of this entity's parent.
    /// (Clamped to the number of children)
    void
    SetChildIndex(int index)
    {
      std::vector<entt::entity>* siblings =
        &Entity(GetComponent<TransformComponent>().parent, m_Registry)
           .GetComponent<TransformComponent>()
           .children;
      auto it =
        std::ranges::find(siblings->begin(), siblings->end(), m_EntityHandle);

      siblings->erase(it);

      if (index >= siblings->size())
      {
        siblings->push_back(m_EntityHandle);
      }
      else
      {
        siblings->insert(siblings->begin() + index, m_EntityHandle);
      }
    }

    /// @brief Returns the index of this entity in the list of children of this
    /// entity's parent.
    /// @return Index of this entity.
    int
    GetChildIndex()
    {
      int index = -1;

      std::vector<entt::entity> siblings =
        Entity(GetComponent<TransformComponent>().parent, m_Registry)
          .GetComponent<TransformComponent>()
          .children;

      // If element was found
      if (auto it =
            std::ranges::find(siblings.begin(), siblings.end(), m_EntityHandle);
          it != siblings.end())
      {
        index = (int)(it - siblings.begin());
      }

      return index;
    }

    /// @brief Returns the entity handle of this entity's parent.
    /// @return Entity handle of the parent.
    entt::entity
    GetParent()
    {
      return GetComponent<TransformComponent>().parent;
    }

    /// @brief Returns the list of this entity's children.
    /// @return Pointer to the list of child entities.
    std::vector<entt::entity>*
    GetChildren()
    {
      return &(GetComponent<TransformComponent>().children);
    }

    entt::entity
    GetHandle() const
    {
      return m_EntityHandle;
    }

  private:
    /// @brief The entity handle of this entity.
    entt::entity m_EntityHandle{ entt::null };

    /// @brief Pointer to a holder of an ECS registry.
    std::shared_ptr<entt::registry> m_Registry;
  };
}