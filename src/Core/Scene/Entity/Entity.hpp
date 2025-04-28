#pragma once

#include "Core/GenericComponents.hpp"
#include "Core/Scene/Components/SceneComponents.hpp"
#include "Core/UUID.hpp"
#include "Utilities/ISerializable.hpp"
#include <entt/entt.hpp>

namespace Dwarf
{

  /// @brief Class wrapping around an entity, providing gameobject
  /// functionality.
  class Entity : ISerializable
  {
  private:
    /// @brief The entity handle of this entity.
    entt::entity mEntityHandle{ entt::null };

    /// @brief Pointer to a holder of an ECS registry.
    std::reference_wrapper<entt::registry> mRegistry;

  public:
    Entity(entt::entity handle, entt::registry& registry);
    // Copy constructor
    Entity(const Entity& other) = default;

    // Move operator
    auto
    operator=(Entity&& other) noexcept -> Entity&
    {
      mEntityHandle = other.mEntityHandle;
      mRegistry.get() = std::move(other.mRegistry.get());
      return *this;
    }

    /// @brief Adds a component to the entity.
    /// @tparam T Type of component.
    /// @tparam ...Args Component arguments.
    /// @param ...args Components to add.
    /// @return The added component.
    template<typename T, typename... Args>
    auto
    AddComponent(Args&&... args) -> T&
    {
      // TODO: Check component requirements
      return mRegistry.get().emplace<T>(mEntityHandle,
                                        std::forward<Args>(args)...);
    }

    /// @brief Retrieves a component of the Entity if present.
    /// @tparam T Type of component.
    /// @return The found component.
    template<typename T>
    auto
    GetComponent() -> T&
    {
      // TODO: Check if component present
      return mRegistry.get().get<T>(mEntityHandle);
    }

    template<typename T>
    auto
    GetComponentHandle() -> T
    {
      return T(mRegistry, mEntityHandle);
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
        mRegistry.get().remove<T>(mEntityHandle);
      }
    }

    /// @brief Checks if the entity has a component assigned.
    /// @tparam T Type of component to check for.
    /// @return True if component is present, false if not.
    template<typename T>
    [[nodiscard]] auto
    HasComponent() const -> bool
    {
      return mRegistry.get().try_get<T>(mEntityHandle) != nullptr;
    }

    /// @brief Retrieves the UID of the entity.
    /// @return The UID.
    [[nodiscard]] auto
    GetUID() const -> const UUID&
    {
      return mRegistry.get().get<IDComponent>(mEntityHandle).getId();
    }

    operator bool() const { return (std::uint32_t)mEntityHandle != 0; }
    auto
    operator==(const Entity& b) -> bool
    {
      return mEntityHandle == b.mEntityHandle;
    }

    /// @brief Sets a new parent for this entity.
    /// @param entity The entity handle of the parent-to-be.
    void
    SetParent(entt::entity entity)
    {
      auto& transform = GetComponent<TransformComponent>();
      auto  newParent = Entity(entity, mRegistry.get());

      if (transform.GetParent() != entt::null)
      {
        auto oldParent = Entity(transform.GetParent(), mRegistry.get());
        oldParent.RemoveChild(mEntityHandle);
      }

      transform.GetParent() = entity;
      if (mRegistry.get().valid(entity))
      {
        newParent.AddChild(mEntityHandle);
      }
    }

    /// @brief Adds a new child entity to this entity.
    /// @param entity Entity handle of the new child.
    void
    AddChild(entt::entity entity)
    {
      auto& transform = GetComponent<TransformComponent>();

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
      auto& transform = GetComponent<TransformComponent>();

      auto iterator = std::ranges::find(
        transform.GetChildren().begin(), transform.GetChildren().end(), entity);
      if (iterator != transform.GetChildren().end())
      {
        transform.GetChildren().erase(iterator);
      }
    }

    /// @brief Sets the position this entity should take between its siblings.
    /// @param index Index for the list of childrens of this entity's parent.
    /// (Clamped to the number of children)
    void
    SetChildIndex(int index)
    {
      auto& transform = GetComponent<TransformComponent>();

      std::vector<entt::entity>* siblings =
        &Entity(transform.GetParent(), mRegistry.get())
           .GetComponent<TransformComponent>()
           .GetChildren();
      auto iterator =
        std::ranges::find(siblings->begin(), siblings->end(), mEntityHandle);

      siblings->erase(iterator);

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
    auto
    GetChildIndex() -> int
    {
      auto& transform = GetComponent<TransformComponent>();

      int index = -1;

      std::vector<entt::entity> siblings =
        Entity(transform.GetParent(), mRegistry.get())
          .GetComponent<TransformComponent>()
          .GetChildren();

      // If element was found
      if (auto iterator =
            std::ranges::find(siblings.begin(), siblings.end(), mEntityHandle);
          iterator != siblings.end())
      {
        index = (int)(iterator - siblings.begin());
      }

      return index;
    }

    /// @brief Returns the entity handle of this entity's parent.
    /// @return Entity handle of the parent.
    [[nodiscard]] auto
    GetParent() const -> const entt::entity&
    {
      return mRegistry.get().get<TransformComponent>(mEntityHandle).GetParent();
    }

    /// @brief Returns the list of this entity's children.
    /// @return Pointer to the list of child entities.
    [[nodiscard]] auto
    GetChildren() const -> const std::vector<entt::entity>&
    {
      return mRegistry.get()
        .get<TransformComponent>(mEntityHandle)
        .GetChildren();
    }

    [[nodiscard]] auto
    GetHandle() const -> entt::entity
    {
      return mEntityHandle;
    }

    auto
    Serialize() -> nlohmann::json override
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

      for (const auto& child : GetChildren())
      {
        serializedEntity["children"][childCount] =
          Entity(child, mRegistry.get()).Serialize();
        childCount++;
      }

      return serializedEntity;
    }
  };
}