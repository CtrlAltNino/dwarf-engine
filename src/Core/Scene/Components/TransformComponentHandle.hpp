#pragma once

#include "Core/GenericComponents.hpp"
#include "Core/Scene/Components/TransformComponent.hpp"

namespace Dwarf
{
  class TransformComponentHandle
  {
  private:
    entt::registry* mRegistry;
    entt::entity    mEntity;

  public:
    TransformComponentHandle(entt::registry* reg, entt::entity ent)
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
      return mRegistry->get<TransformComponent>(mEntity).Position;
    }

    void
    SetPosition(glm::vec3 position)
    {
      mRegistry->patch<TransformComponent>(
        mEntity,
        [position](TransformComponent& component) mutable
        { component.SetPosition(position); });
    }

    [[nodiscard]] auto
    GetEulerAngles() const -> glm::vec3
    {
      return mRegistry->get<TransformComponent>(mEntity).Rotation;
    }

    void
    SetEulerAngles(glm::vec3 rotation)
    {
      mRegistry->patch<TransformComponent>(
        mEntity,
        [rotation](TransformComponent& component) mutable
        { component.SetEulerAngles(rotation); });
    }

    [[nodiscard]] auto
    GetScale() const -> glm::vec3
    {
      return mRegistry->get<TransformComponent>(mEntity).Scale;
    }

    void
    SetScale(glm::vec3 scale)
    {
      mRegistry->patch<TransformComponent>(
        mEntity,
        [scale](TransformComponent& component) mutable
        { component.SetScale(scale); });
    }

    [[nodiscard]] auto
    GetParent() const -> entt::entity
    {
      return mRegistry->get<TransformComponent>(mEntity).Parent;
    }

    void
    SetParent(entt::entity parent)
    {
      mRegistry->patch<TransformComponent>(
        mEntity,
        [parent](TransformComponent& component) mutable
        { component.Parent = parent; });
    }

    [[nodiscard]] auto
    GetChildren() const -> std::vector<entt::entity>
    {
      return mRegistry->get<TransformComponent>(mEntity).Children;
    }

    void
    SetChildren(std::vector<entt::entity> children)
    {
      mRegistry->patch<TransformComponent>(
        mEntity,
        [children](TransformComponent& component) mutable
        { component.Children = children; });
    }

    /// @brief Returns the model matrix of the entity. A composite matrix of the
    /// translation, scale and rotation matrices.
    /// @return The model matrix as a 4x4 matrix.
    [[nodiscard]] auto
    GetModelMatrix() const -> glm::mat4x4
    {
      return mRegistry->get<TransformComponent>(mEntity).GetModelMatrix();
    }

    /// @brief Returns the view matrix of the entity. A composite matrix of the
    /// translation, scale and rotation matrices.
    /// @return The view matrix as a 4x4 matrix.
    [[nodiscard]] auto
    GetViewMatrix() const -> glm::mat4x4
    {
      return mRegistry->get<TransformComponent>(mEntity).GetViewMatrix();
    }

    void
    SetMatrix(glm::mat4 matrix)
    {
      mRegistry->patch<TransformComponent>(
        mEntity,
        [matrix](TransformComponent& component) mutable
        { component.SetMatrix(matrix); });
    }
  };
}