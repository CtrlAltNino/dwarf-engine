#include "Camera.h"
#include "Core/Scene/Camera/ICamera.h"

namespace Dwarf
{

  // ========== Makros ==========

#define DEFAULT_CAMERA_POSITION glm::vec3(0, 2, 15)
#define DEFAULT_CAMERA_ROTATION glm::vec3(30, 0, 0)

  // ========== Constructors ==========
  Camera::Camera(std::shared_ptr<IInputManager> inputManager,
                 CameraProperties               properties)
    : mInputManager(inputManager)
    , mProperties(properties)
  {
  }

  Camera::Camera(std::shared_ptr<IInputManager> inputManager,
                 nlohmann::json                 json)
    : mInputManager(inputManager)
    , mProperties(CameraProperties())
  {
    mProperties.Transform = TransformComponent(json["transform"]);
    mProperties.Fov = json["fov"];
    mProperties.NearPlane = json["nearPlane"];
    mProperties.FarPlane = json["farPlane"];
    mProperties.AspectRatio = json["aspectRatio"];
    mProperties.Sensitivity = json["sensitivity"];
    mProperties.MovementSpeed = json["movementSpeed"];
  }

  glm::mat4x4
  Camera::GetViewMatrix() const
  {
    glm::mat4 rot =
      glm::rotate(glm::mat4(1.0f),
                  mProperties.Transform.GetEulerAngles().x * DEG_2_RAD,
                  glm::vec3(1.0f, 0.0f, 0.0f)) *
      glm::rotate(glm::mat4(1.0f),
                  mProperties.Transform.GetEulerAngles().y * DEG_2_RAD,
                  glm::vec3(0.0f, 1.0f, 0.0f));

    return rot * glm::translate(glm::mat4(1.0f),
                                -mProperties.Transform.GetPosition());
  }

  glm::mat4x4
  Camera::GetProjectionMatrix() const
  {
    return glm::perspective(glm::radians(mProperties.Fov),
                            mProperties.AspectRatio,
                            mProperties.NearPlane,
                            mProperties.FarPlane);
  }

  // ========== Camera Functions ==========
  void
  Camera::OnUpdate(double deltaTime)
  {
    using enum KEYCODE;
    glm::ivec2 deltaMousePos = mInputManager->GetMouseDelta();

    if (deltaMousePos.length() > 0)
    {
      float     yAngle = (float)deltaMousePos.x * mProperties.Sensitivity;
      float     xAngle = (float)deltaMousePos.y * mProperties.Sensitivity;
      glm::mat4 mat =
        glm::rotate(glm::mat4(1.0f), xAngle * DEG_2_RAD, glm::vec3(1, 0, 0));

      glm::vec3 rot;

      rot.x = RAD_2_DEG * atan2f(mat[1][2], mat[2][2]);

      mProperties.Transform.GetEulerAngles().x += rot.x;
      mProperties.Transform.GetEulerAngles().x = std::min(
        std::max(mProperties.Transform.GetEulerAngles().x, -90.0f), 90.0f);

      mat =
        glm::rotate(glm::mat4(1.0f), yAngle * DEG_2_RAD, glm::vec3(0, 1, 0));
      rot.y = RAD_2_DEG *
              atan2f(-mat[0][2],
                     sqrtf(mat[1][2] * mat[1][2] + mat[2][2] * mat[2][2]));

      mProperties.Transform.GetEulerAngles().y += rot.y;
    }

    glm::vec3 movementVector = {
      (mInputManager->GetKey(A) ? -1 : 0) + (mInputManager->GetKey(D) ? 1 : 0),
      (mInputManager->GetKey(Q) ? -1 : 0) + (mInputManager->GetKey(E) ? 1 : 0),
      (mInputManager->GetKey(W) ? -1 : 0) + (mInputManager->GetKey(S) ? 1 : 0)
    };

    glm::mat4 rotMat =
      glm::rotate(glm::mat4(1.0f),
                  mProperties.Transform.GetEulerAngles().x * DEG_2_RAD,
                  glm::vec3(1.0f, 0.0f, 0.0f)) *
      glm::rotate(glm::mat4(1.0f),
                  mProperties.Transform.GetEulerAngles().y * DEG_2_RAD,
                  glm::vec3(0.0f, 1.0f, 0.0f));

    if (glm::length(movementVector) > 0)
    {
      movementVector = glm::normalize(movementVector);
      movementVector *= deltaTime * mProperties.MovementSpeed *
                        (mInputManager->GetKey(KEYCODE::LEFT_SHIFT) ? 2 : 1);

      glm::vec4 deltaVec4 =
        glm::inverse(rotMat) *
        glm::vec4(movementVector.x, movementVector.y, movementVector.z, 1.0f);

      mProperties.Transform.GetPosition() +=
        glm::vec3(deltaVec4.x, deltaVec4.y, deltaVec4.z);
    }

    deltaMousePos = glm::vec2(0);
  }

  glm::vec3
  Camera::ScreenToWorld(glm::vec2 const& screenPos,
                        glm::vec2 const& viewport) const
  {
    float     x = (2.0f * screenPos.x) / viewport.x - 1.0f;
    float     y = 1.0f - (2.0f * screenPos.y) / viewport.y;
    float     z = 1.0f;
    glm::vec3 ray_nds = glm::vec3(x, y, z);
    glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
    glm::vec4 ray_eye = glm::inverse(GetProjectionMatrix()) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
    glm::vec3 ray_wor = glm::vec3(glm::inverse(GetViewMatrix()) * ray_eye);
    ray_wor = glm::normalize(ray_wor);
    return ray_wor;
  }

  CameraProperties&
  Camera::GetProperties()
  {
    return mProperties;
  }

  nlohmann::json
  Camera::Serialize()
  {
    nlohmann::json j;
    j["transform"] = mProperties.Transform.Serialize();
    j["fov"] = mProperties.Fov;
    j["nearPlane"] = mProperties.NearPlane;
    j["farPlane"] = mProperties.FarPlane;
    j["aspectRatio"] = mProperties.AspectRatio;
    j["sensitivity"] = mProperties.Sensitivity;
    j["movementSpeed"] = mProperties.MovementSpeed;
    return j;
  }
}