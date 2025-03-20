#pragma once
#include "Utilities/ISerializable.h"
#include "pch.h"

#include "Core/Base.h"
#include "Core/GenericComponents.h"
#include "Core/Scene/Components/SceneComponents.h"
#include "Core/UUID.h"
#include <entt/entt.hpp>

namespace Dwarf
{

  /// @brief Class wrapping around an entity, providing gameobject
  /// functionality.
  class Entity : ISerializable
  {
  public:
    Entity(entt::entity handle, entt::registry& registry);
    // Copy constructor
    Entity(const Entity& other) = default;

    // Move operator
    Entity&
    operator=(Entity&& other) noexcept
    {
      mEntityHandle = other.mEntityHandle;
      mRegistry = std::move(other.mRegistry);
      return *this;
    }

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
      return mRegistry.emplace<T>(mEntityHandle, std::forward<Args>(args)...);
    }

    /// @brief Retrieves a component of the Entity if present.
    /// @tparam T Type of component.
    /// @return The found component.
    template<typename T>
    T&
    GetComponent()
    {
      // TODO: Check if component present
      return mRegistry.get<T>(mEntityHandle);
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
        mRegistry.remove<T>(mEntityHandle);
      }
    }

    /// @brief Checks if the entity has a component assigned.
    /// @tparam T Type of component to check for.
    /// @return True if component is present, false if not.
    template<typename T>
    bool
    HasComponent()
    {
      return mRegistry.try_get<T>(mEntityHandle) != nullptr;
    }

    /// @brief Retrieves the UID of the entity.
    /// @return The UID.
    const UUID&
    GetUID() const
    {
      return mRegistry.get<IDComponent>(mEntityHandle).getId();
    }

    operator bool() const { return (std::uint32_t)mEntityHandle != 0; }
    bool
    operator==(const Entity& b)
    {
      return mEntityHandle == b.mEntityHandle;
    }

    /// @brief Sets a new parent for this entity.
    /// @param entity The entity handle of the parent-to-be.
    void
    SetParent(entt::entity entity)
    {
      TransformComponent& transform = GetComponent<TransformComponent>();
      auto                newParent = Entity(entity, mRegistry);

      if (transform.GetParent() != entt::null)
      {
        auto oldParent = Entity(transform.GetParent(), mRegistry);
        oldParent.RemoveChild(mEntityHandle);
      }

      transform.GetParent() = entity;
      if (mRegistry.valid(entity))
      {
        newParent.AddChild(mEntityHandle);
      }
    }

    /// @brief Adds a new child entity to this entity.
    /// @param entity Entity handle of the new child.
    void
    AddChild(entt::entity entity)
    {
      TransformComponent& transform = GetComponent<TransformComponent>();

      if (transform.GetChildren().empty() ||
          (std::ranges::find(transform.GetChildren().begin(),
                             transform.GetChildren().end(),
                             entity) == transform.GetChildren().end()))
      {
        transform.GetChildren().push_back(entity);
      }
    }

    /// @brief Removes a child from the list of children.
    /// @param entity Entity handle of the entity to remove.
    void
    RemoveChild(entt::entity entity)
    {
      TransformComponent& transform = GetComponent<TransformComponent>();

      auto it = std::ranges::find(
        transform.GetChildren().begin(), transform.GetChildren().end(), entity);
      if (it != transform.GetChildren().end())
      {
        transform.GetChildren().erase(it);
      }
    }

    /// @brief Sets the position this entity should take between its siblings.
    /// @param index Index for the list of childrens of this entity's parent.
    /// (Clamped to the number of children)
    void
    SetChildIndex(int index)
    {
      TransformComponent& transform = GetComponent<TransformComponent>();

      std::vector<entt::entity>* siblings =
        &Entity(transform.GetParent(), mRegistry)
           .GetComponent<TransformComponent>()
           .GetChildren();
      auto it =
        std::ranges::find(siblings->begin(), siblings->end(), mEntityHandle);

      siblings->erase(it);

      if (index >= siblings->size())
      {
        siblings->push_back(mEntityHandle);
      }
      else
      {
        siblings->insert(siblings->begin() + index, mEntityHandle);
      }
    }

    /// @brief Returns the index of this entity in the list of children of this
    /// entity's parent.
    /// @return Index of this entity.
    int
    GetChildIndex()
    {
      TransformComponent& transform = GetComponent<TransformComponent>();

      int index = -1;

      std::vector<entt::entity> siblings =
        Entity(transform.GetParent(), mRegistry)
          .GetComponent<TransformComponent>()
          .GetChildren();

      // If element was found
      if (auto it =
            std::ranges::find(siblings.begin(), siblings.end(), mEntityHandle);
          it != siblings.end())
      {
        index = (int)(it - siblings.begin());
      }

      return index;
    }

    /// @brief Returns the entity handle of this entity's parent.
    /// @return Entity handle of the parent.
    const entt::entity&
    GetParent() const
    {
      return mRegistry.get<TransformComponent>(mEntityHandle).GetParent();
    }

    /// @brief Returns the list of this entity's children.
    /// @return Pointer to the list of child entities.
    const std::vector<entt::entity>&
    GetChildren() const
    {
      return mRegistry.get<TransformComponent>(mEntityHandle).GetChildren();
    }

    entt::entity
    GetHandle() const
    {
      return mEntityHandle;
    }

    nlohmann::json
    Serialize() override
    {
      nlohmann::json serializedEntity;

      serializedEntity["guid"] = GetComponent<IDComponent>().getId().toString();
      serializedEntity["name"] = GetComponent<NameComponent>().Name;

      serializedEntity["TransformComponent"] =
        GetComponent<TransformComponent>().Serialize();

      if (HasComponent<LightComponent>())
      {
        serializedEntity["LightComponent"] =
          GetComponent<LightComponent>().Serialize();
      }

      if (HasComponent<MeshRendererComponent>())
      {
        serializedEntity["MeshRendererComponent"] =
          GetComponent<MeshRendererComponent>().Serialize();
      }

      int childCount = 0;

      for (auto& child : GetChildren())
      {
        serializedEntity["children"][childCount] =
          Entity(child, mRegistry).Serialize();
        childCount++;
      }

      return serializedEntity;
    }

  private:
    /// @brief The entity handle of this entity.
    entt::entity mEntityHandle{ entt::null };

    /// @brief Pointer to a holder of an ECS registry.
    entt::registry& mRegistry;
  };
}