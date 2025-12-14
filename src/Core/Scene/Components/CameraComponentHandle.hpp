#pragma once

#include "Core/GenericComponents.hpp"
#include "Core/Scene/Camera/ICamera.hpp"
#include "Core/Scene/Components/CameraComponent.hpp"
#include "Core/Scene/Components/TransformComponent.hpp"

namespace Dwarf
{
  class CameraComponentHandle
  {
  private:
    entt::registry* mRegistry;
    entt::entity    mEntity;

  public:
    CameraComponentHandle(entt::registry* reg, entt::entity ent)
      : mRegistry(reg)
      , mEntity(ent)
    {
    }

    auto
    GetHandle() -> entt::entity
    {
      return mEntity;
    }

    [[nodiscard]] auto
    GetProperties() const -> CameraProperties&
    {
      return mRegistry->get<CameraComponent>(mEntity).Camera->GetProperties();
    }

    /// @brief Returns the type of light this component represents.
    /// @return The light type.
    [[nodiscard]] auto
    GetViewMatrix() const -> glm::mat4x4
    {
      auto&     transform = mRegistry->get<TransformComponent>(mEntity);
      glm::mat4 rot = glm::rotate(glm::mat4(1.0F),
                                  transform.GetEulerAngles().x * DEG_2_RAD,
                                  glm::vec3(1.0F, 0.0F, 0.0F)) *
                      glm::rotate(glm::mat4(1.0F),
                                  transform.GetEulerAngles().y * DEG_2_RAD,
                                  glm::vec3(0.0F, 1.0F, 0.0F));

      return rot * glm::translate(glm::mat4(1.0F), -transform.GetPosition());
    }

    [[nodiscard]] auto
    GetProjectionMatrix(float aspectRatio) const -> glm::mat4x4
    {
      return mRegistry->get<CameraComponent>(mEntity)
        .Camera->GetProjectionMatrix(aspectRatio);
    }

    auto
    GetTransform() -> TransformComponent&
    {
      return mRegistry->get<TransformComponent>(mEntity);
    }

    auto
    GetTransform() const -> TransformComponent&
    {
      return mRegistry->get<TransformComponent>(mEntity);
    }

    auto
    GetProperties() -> CameraProperties&
    {
      return mRegistry->get<CameraComponent>(mEntity).Camera->GetProperties();
    }
  };
}