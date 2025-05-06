#pragma once

#include "Core/GenericComponents.hpp"
#include "Core/Scene/Components/SceneComponents.hpp"

namespace Dwarf
{
  class TransformComponentHandle
  {
  private:
    std::reference_wrapper<entt::registry> mRegistry;
    entt::entity                           mEntity;

  public:
    TransformComponentHandle(entt::registry& reg, entt::entity ent)
      : mRegistry(reg)
      , mEntity(ent)
    {
    }

    auto
    GetHandle() -> entt::entity
    {
      return mEntity;
    }

    // ========== Getters ==========

    [[nodiscard]] auto
    GetPosition() const -> glm::vec3
    {
      return mRegistry.get().get<TransformComponent>(mEntity).GetPosition();
    }

    void
    SetPosition(glm::vec3 position)
    {
      mRegistry.get().patch<TransformComponent>(
        mEntity,
        [position](TransformComponent& component) mutable
        { component.SetPosition(position); });
    }

    [[nodiscard]] auto
    GetEulerAngles() const -> glm::vec3
    {
      return mRegistry.get().get<TransformComponent>(mEntity).GetEulerAngles();
    }

    void
    SetEulerAngles(glm::vec3 rotation)
    {
      mRegistry.get().patch<TransformComponent>(
        mEntity,
        [rotation](TransformComponent& component) mutable
        { component.SetEulerAngles(rotation); });
    }

    [[nodiscard]] auto
    GetScale() const -> glm::vec3
    {
      return mRegistry.get().get<TransformComponent>(mEntity).GetScale();
    }

    void
    SetScale(glm::vec3 scale)
    {
      mRegistry.get().patch<TransformComponent>(
        mEntity,
        [scale](TransformComponent& component) mutable
        { component.SetScale(scale); });
    }

    [[nodiscard]] auto
    GetParent() const -> entt::entity
    {
      return mRegistry.get().get<TransformComponent>(mEntity).GetParent();
    }

    void
    SetParent(entt::entity parent)
    {
      mRegistry.get().patch<TransformComponent>(
        mEntity,
        [parent](TransformComponent& component) mutable
        { component.Parent = parent; });
    }

    [[nodiscard]] auto
    GetChildren() const -> std::vector<entt::entity>
    {
      return mRegistry.get().get<TransformComponent>(mEntity).Children;
    }

    void
    SetChildren(std::vector<entt::entity> children)
    {
      mRegistry.get().patch<TransformComponent>(
        mEntity,
        [children](TransformComponent& component) mutable
        { component.Children = children; });
    }

    /// @brief Returns the model matrix of the entity. A composite matrix of the
    /// translation, scale and rotation matrices.
    /// @return The model matrix as a 4x4 matrix.
    [[nodiscard]] auto
    GetMatrix() const -> glm::mat4x4
    {
      return mRegistry.get().get<TransformComponent>(mEntity).GetMatrix();
    }

    void
    SetMatrix(glm::mat4 matrix)
    {
      mRegistry.get().patch<TransformComponent>(
        mEntity,
        [matrix](TransformComponent& component) mutable
        { component.SetMatrix(matrix); });
    }
  };
}