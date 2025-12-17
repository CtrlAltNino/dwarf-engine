#include "pch.hpp"

#include "CameraSystem.hpp"

namespace Dwarf
{
  CameraSystem::CameraSystem(std::shared_ptr<IDwarfLogger>  logger,
                             std::shared_ptr<IInputManager> inputManager)
    : mLogger(std::move(logger))
    , mInputManager(std::move(inputManager))
  {
  }

  CameraSystem::~CameraSystem() {}

  void
  CameraSystem::SetCamera(CameraComponentHandle handle)
  {
    mCamera = handle;
  }

  auto
  CameraSystem::GetCamera() -> CameraComponentHandle
  {
    return mCamera.value();
  }

  auto
  CameraSystem::HasCamera() const -> bool
  {
    return mCamera.has_value();
  }

  [[nodiscard]] auto
  CameraSystem::GetViewMatrix() const -> glm::mat4x4
  {
    if (mCamera.has_value())
    {
      return mCamera->GetViewMatrix();
    }

    return { 0 };
  }

  [[nodiscard]] auto
  CameraSystem::GetProjectionMatrix(float aspectRatio) const -> glm::mat4x4
  {
    if (mCamera.has_value())
    {
      return mCamera->GetProjectionMatrix(aspectRatio);
    }

    return { 0 };
  }

  [[nodiscard]] auto
  CameraSystem::GetPosition() const -> glm::vec3
  {
    if (mCamera.has_value())
    {
      return mCamera->GetTransform().GetPosition();
    }

    return { 0, 0, 0 };
  }

  auto
  CameraSystem::GetNearplane() const -> float
  {
    return mCamera->GetProperties().NearPlane;
  }

  auto
  CameraSystem::GetFarplane() const -> float
  {
    return mCamera->GetProperties().FarPlane;
  }

  auto
  CameraSystem::GetFov() const -> float
  {
    return mCamera->GetProperties().Fov;
  }

  void
  CameraSystem::SetNearplane(float nearplane)
  {
    mCamera->GetProperties().NearPlane = nearplane;
  }

  void
  CameraSystem::SetFarplane(float farplane)
  {
    mCamera->GetProperties().FarPlane = farplane;
  }

  void
  CameraSystem::SetFov(float fov)
  {
    mCamera->GetProperties().Fov = fov;
  }

  void
  CameraSystem::OnUpdate(double deltaTime)
  {
    using enum KEYCODE;
    glm::ivec2 deltaMousePos = mInputManager->GetMouseDelta();

    if (deltaMousePos.length() > 0)
    {
      float     yAngle = (float)deltaMousePos.x * mProperties.Sensitivity;
      float     xAngle = (float)deltaMousePos.y * mProperties.Sensitivity;
      glm::mat4 mat =
        glm::rotate(glm::mat4(1.0F), xAngle * DEG_2_RAD, glm::vec3(1, 0, 0));

      glm::vec3 rot;

      rot.x = RAD_2_DEG * atan2f(mat[1][2], mat[2][2]);

      glm::vec3 updatedEulerAngles = mCamera->GetTransform().GetEulerAngles();

      updatedEulerAngles.x += rot.x;

      mat =
        glm::rotate(glm::mat4(1.0F), yAngle * DEG_2_RAD, glm::vec3(0, 1, 0));
      rot.y = RAD_2_DEG *
              atan2f(-mat[0][2],
                     sqrtf((mat[1][2] * mat[1][2]) + (mat[2][2] * mat[2][2])));

      updatedEulerAngles.y += rot.y;

      mCamera->GetTransform().SetEulerAngles(updatedEulerAngles);
    }

    glm::vec3 movementVector = {
      (mInputManager->GetKey(A) ? -1 : 0) + (mInputManager->GetKey(D) ? 1 : 0),
      (mInputManager->GetKey(Q) ? -1 : 0) + (mInputManager->GetKey(E) ? 1 : 0),
      (mInputManager->GetKey(W) ? -1 : 0) + (mInputManager->GetKey(S) ? 1 : 0)
    };

    glm::mat4 rotMat =
      glm::rotate(glm::mat4(1.0F),
                  mCamera->GetTransform().GetEulerAngles().x * DEG_2_RAD,
                  glm::vec3(1.0F, 0.0F, 0.0F)) *
      glm::rotate(glm::mat4(1.0F),
                  mCamera->GetTransform().GetEulerAngles().y * DEG_2_RAD,
                  glm::vec3(0.0F, 1.0F, 0.0F));

    if (glm::length(movementVector) > 0)
    {
      movementVector = glm::normalize(movementVector);
      movementVector *= deltaTime * mProperties.MovementSpeed *
                        (mInputManager->GetKey(KEYCODE::LEFT_SHIFT) ? 2 : 1);

      glm::vec4 deltaVec4 =
        glm::inverse(rotMat) *
        glm::vec4(movementVector.x, movementVector.y, movementVector.z, 1.0F);

      mCamera->GetTransform().SetPosition(
        mCamera->GetTransform().GetPosition() +
        glm::vec3(deltaVec4.x, deltaVec4.y, deltaVec4.z));
    }

    deltaMousePos = glm::vec2(0);
  }

  [[nodiscard]] auto
  CameraSystem::ScreenToWorld(glm::vec2 const& screenPos,
                              glm::vec2 const& viewport) const -> glm::vec3
  {
    // Converting the screen coordinates to normalized device coordinates
    float     x = ((2.0F * screenPos.x) / viewport.x) - 1.0F;
    float     y = 1.0F - ((2.0F * screenPos.y) / viewport.y);
    float     z = 1.0F;
    glm::vec3 rayNds = glm::vec3(x, y, z);

    // NDC to clip space
    glm::vec4 rayClip = glm::vec4(rayNds.x, rayNds.y, -1.0, 1.0);

    // Clip space to Eye (View) space
    glm::vec4 rayEye =
      glm::inverse(GetProjectionMatrix(viewport.x / viewport.y)) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);

    // Eye space to world space
    glm::vec3 rayWor = glm::vec3(glm::inverse(GetViewMatrix()) * rayEye);

    // Normalization
    rayWor = glm::normalize(rayWor);
    return rayWor;
  }

  auto
  CameraSystem::Serialize() -> nlohmann::json
  {
    nlohmann::json j;
    // TODO
    return j;
  }
}