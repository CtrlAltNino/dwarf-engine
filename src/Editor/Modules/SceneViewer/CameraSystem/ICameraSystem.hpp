#pragma once

#include "Core/Scene/Components/CameraComponentHandle.hpp"

namespace Dwarf
{
  struct CameraSystemProperties
  {
    float Sensitivity = 0.15F;
    float MovementSpeed = 4.0F;
  };

  class ICameraSystem : public ISerializable
  {
  public:
    virtual ~ICameraSystem() = default;

    virtual void
    SetCamera(CameraComponentHandle handle) = 0;

    virtual auto
    GetCamera() -> CameraComponentHandle = 0;

    [[nodiscard]] virtual auto
    HasCamera() const -> bool = 0;

    [[nodiscard]] virtual auto
    GetViewMatrix() const -> glm::mat4x4 = 0;

    [[nodiscard]] virtual auto
    GetProjectionMatrix(float aspectRatio) const -> glm::mat4x4 = 0;

    [[nodiscard]] virtual auto
    GetPosition() const -> glm::vec3 = 0;

    [[nodiscard]] virtual auto
    GetNearplane() const -> float = 0;

    [[nodiscard]] virtual auto
    GetFarplane() const -> float = 0;

    [[nodiscard]] virtual auto
    GetFov() const -> float = 0;

    virtual void
    SetNearplane(float nearplane) = 0;

    virtual void
    SetFarplane(float farplane) = 0;

    virtual void
    SetFov(float fov) = 0;

    virtual void
    OnUpdate(double deltaTime) = 0;

    [[nodiscard]] virtual auto
    ScreenToWorld(glm::vec2 const& screenPosition,
                  glm::vec2 const& viewport) const -> glm::vec3 = 0;

    auto
    Serialize() -> nlohmann::json override = 0;
  };
}