#include "pch.hpp"

#include "Camera.hpp"

namespace Dwarf
{

  // ========== Makros ==========

#define DEFAULT_CAMERA_POSITION glm::vec3(0, 2, 15)
#define DEFAULT_CAMERA_ROTATION glm::vec3(30, 0, 0)

  // ========== Constructors ==========
  Camera::Camera(std::shared_ptr<IDwarfLogger>  logger,
                 std::shared_ptr<IInputManager> inputManager,
                 CameraProperties               properties)
    : mLogger(std::move(logger))
    , mInputManager(std::move(inputManager))
    , mProperties(std::move(properties))
  {
    mLogger->LogDebug(Log("Camera created.", "Camera"));
  }

  Camera::Camera(std::shared_ptr<IDwarfLogger>  logger,
                 std::shared_ptr<IInputManager> inputManager,
                 const nlohmann::json&          serializedCameraData)
    : mLogger(std::move(logger))
    , mInputManager(std::move(inputManager))
    , mProperties(CameraProperties())
  {
    mProperties.Transform =
      TransformComponent(serializedCameraData["transform"]);

    if (serializedCameraData.contains("fov"))
    {
      mProperties.Fov = serializedCameraData["fov"];
    }

    if (serializedCameraData.contains("nearPlane"))
    {
      mProperties.NearPlane = serializedCameraData["nearPlane"];
    }

    if (serializedCameraData.contains("farPlane"))
    {
      mProperties.FarPlane = serializedCameraData["farPlane"];
    }

    if (serializedCameraData.contains("aspectRatio"))
    {
      mProperties.AspectRatio = serializedCameraData["aspectRatio"];
    }

    if (serializedCameraData.contains("sensitivity"))
    {
      mProperties.Sensitivity = serializedCameraData["sensitivity"];
    }

    if (serializedCameraData.contains("movementSpeed"))
    {
      mProperties.MovementSpeed = serializedCameraData["movementSpeed"];
    }

    mLogger->LogDebug(Log("Camera created.", "Camera"));
  }

  Camera::~Camera()
  {
    mLogger->LogDebug(Log("Camera destroyed.", "Camera"));
  }

  auto
  Camera::GetViewMatrix() const -> glm::mat4x4
  {
    glm::mat4 rot =
      glm::rotate(glm::mat4(1.0F),
                  mProperties.Transform.GetEulerAngles().x * DEG_2_RAD,
                  glm::vec3(1.0F, 0.0F, 0.0F)) *
      glm::rotate(glm::mat4(1.0F),
                  mProperties.Transform.GetEulerAngles().y * DEG_2_RAD,
                  glm::vec3(0.0F, 1.0F, 0.0F));

    return rot * glm::translate(glm::mat4(1.0F),
                                -mProperties.Transform.GetPosition());
  }

  auto
  Camera::GetProjectionMatrix() const -> glm::mat4x4
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
        glm::rotate(glm::mat4(1.0F), xAngle * DEG_2_RAD, glm::vec3(1, 0, 0));

      glm::vec3 rot;

      rot.x = RAD_2_DEG * atan2f(mat[1][2], mat[2][2]);

      glm::vec3 updatedEulerAngles = mProperties.Transform.GetEulerAngles();

      updatedEulerAngles.x += rot.x;
      updatedEulerAngles.x =
        std::min(std::max(updatedEulerAngles.x, -90.0F), 90.0F);

      mat =
        glm::rotate(glm::mat4(1.0F), yAngle * DEG_2_RAD, glm::vec3(0, 1, 0));
      rot.y = RAD_2_DEG *
              atan2f(-mat[0][2],
                     sqrtf((mat[1][2] * mat[1][2]) + (mat[2][2] * mat[2][2])));

      updatedEulerAngles.y += rot.y;

      mProperties.Transform.SetEulerAngles(updatedEulerAngles);
    }

    glm::vec3 movementVector = {
      (mInputManager->GetKey(A) ? -1 : 0) + (mInputManager->GetKey(D) ? 1 : 0),
      (mInputManager->GetKey(Q) ? -1 : 0) + (mInputManager->GetKey(E) ? 1 : 0),
      (mInputManager->GetKey(W) ? -1 : 0) + (mInputManager->GetKey(S) ? 1 : 0)
    };

    glm::mat4 rotMat =
      glm::rotate(glm::mat4(1.0F),
                  mProperties.Transform.GetEulerAngles().x * DEG_2_RAD,
                  glm::vec3(1.0F, 0.0F, 0.0F)) *
      glm::rotate(glm::mat4(1.0F),
                  mProperties.Transform.GetEulerAngles().y * DEG_2_RAD,
                  glm::vec3(0.0F, 1.0F, 0.0F));

    if (glm::length(movementVector) > 0)
    {
      movementVector = glm::normalize(movementVector);
      movementVector *= deltaTime * mProperties.MovementSpeed *
                        (mInputManager->GetKey(KEYCODE::LEFT_SHIFT) ? 2 : 1);

      glm::vec4 deltaVec4 =
        glm::inverse(rotMat) *
        glm::vec4(movementVector.x, movementVector.y, movementVector.z, 1.0F);

      mProperties.Transform.GetPosition() +=
        glm::vec3(deltaVec4.x, deltaVec4.y, deltaVec4.z);
    }

    deltaMousePos = glm::vec2(0);
  }

  auto
  Camera::ScreenToWorld(glm::vec2 const& screenPos,
                        glm::vec2 const& viewport) const -> glm::vec3
  {
    float     x = ((2.0F * screenPos.x) / viewport.x) - 1.0F;
    float     y = 1.0F - ((2.0F * screenPos.y) / viewport.y);
    float     z = 1.0F;
    glm::vec3 rayNds = glm::vec3(x, y, z);
    glm::vec4 rayClip = glm::vec4(rayNds.x, rayNds.y, -1.0, 1.0);
    glm::vec4 rayEye = glm::inverse(GetProjectionMatrix()) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);
    glm::vec3 rayWor = glm::vec3(glm::inverse(GetViewMatrix()) * rayEye);
    rayWor = glm::normalize(rayWor);
    return rayWor;
  }

  auto
  Camera::GetProperties() -> CameraProperties&
  {
    return mProperties;
  }

  auto
  Camera::Serialize() -> nlohmann::json
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