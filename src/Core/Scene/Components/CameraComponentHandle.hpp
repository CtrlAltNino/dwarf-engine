#pragma once

#include "Core/Scene/Camera/ICamera.hpp"
#include "Core/Scene/Components/CameraComponent.hpp"
#include "Core/Scene/Components/TransformComponent.hpp"

namespace Dwarf
{
  /**
   * @brief Safe handle class to access the camera component of a scene entity
   *
   */
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

    /**
     * @brief Get the entity handle
     *
     * @return entt::entity entt handle of the source entity
     */
    auto
    GetHandle() -> entt::entity
    {
      return mEntity;
    }

    /**
     * @brief Get's a reference to the properties of the camera
     *
     * @return CameraProperties& Reference to the camera properties
     */
    auto
    GetProperties() -> CameraProperties&
    {
      return mRegistry->get<CameraComponent>(mEntity).Camera->GetProperties();
    }

    /**
     * @brief Get's a reference to the properties of the camera
     *
     * @return CameraProperties& Reference to the camera properties
     */
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

    /**
     * @brief Gets the projection matrix of the camera
     *
     * @param aspectRatio Aspect ratio to use for calculating the projection
     * matrix
     * @return glm::mat4x4 Projection matrix
     */
    [[nodiscard]] auto
    GetProjectionMatrix(float aspectRatio) const -> glm::mat4x4
    {
      return mRegistry->get<CameraComponent>(mEntity)
        .Camera->GetProjectionMatrix(aspectRatio);
    }

    /**
     * @brief Returns a reference to the transform of the entity
     *
     * @return TransformComponent& Reference to the transform component
     */
    auto
    GetTransform() -> TransformComponent&
    {
      return mRegistry->get<TransformComponent>(mEntity);
    }

    /**
     * @brief Const access to the transfrom component
     *
     * @return TransformComponent& Reference to the transform component
     */
    [[nodiscard]] auto
    GetTransform() const -> TransformComponent&
    {
      return mRegistry->get<TransformComponent>(mEntity);
    }
  };
}