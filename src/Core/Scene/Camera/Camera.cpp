#include "Camera.h"
#include "Input/IInputManager.h"
#include "Input/InputManager.h"

namespace Dwarf
{

  // ========== Makros ==========

#define DEFAULT_CAMERA_POSITION glm::vec3(0, 2, 15)
#define DEFAULT_CAMERA_ROTATION glm::vec3(30, 0, 0)

  // ========== Constructors ==========
  Camera::Camera(std::shared_ptr<IInputManager> inputManager,
                 CameraProperties               properties)
    : m_InputManager(inputManager)
    , m_Properties(properties)
  {
  }

  Camera::Camera(std::shared_ptr<IInputManager> inputManager,
                 nlohmann::json                 json,
                 CameraProperties               properties)
    : m_InputManager(inputManager)
    , m_Properties(properties)
  {
    m_Properties.Transform = TransformComponent(json["transform"]);
    m_Properties.Fov = json["fov"];
    m_Properties.NearPlane = json["nearPlane"];
    m_Properties.FarPlane = json["farPlane"];
    m_Properties.AspectRatio = json["aspectRatio"];
    m_Properties.Sensitivity = json["sensitivity"];
    m_Properties.MovementSpeed = json["movementSpeed"];
  }

  glm::mat4x4
  Camera::GetViewMatrix() const
  {
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f),
                                m_Properties.Transform.rotation.x * DEG_2_RAD,
                                glm::vec3(1.0f, 0.0f, 0.0f)) *
                    glm::rotate(glm::mat4(1.0f),
                                m_Properties.Transform.rotation.y * DEG_2_RAD,
                                glm::vec3(0.0f, 1.0f, 0.0f));

    return rot * glm::translate(glm::mat4(1.0f),
                                -m_Properties.Transform.getPosition());
  }

  glm::mat4x4
  Camera::GetProjectionMatrix() const
  {
    return glm::perspective(glm::radians(m_Properties.Fov),
                            m_Properties.AspectRatio,
                            m_Properties.NearPlane,
                            m_Properties.FarPlane);
  }

  // ========== Camera Functions ==========
  void
  Camera::OnUpdate(double deltaTime)
  {
    using enum KEYCODE;
    glm::ivec2 deltaMousePos = m_InputManager->GetMouseDelta();

    if (deltaMousePos.length() > 0)
    {
      float     yAngle = (float)deltaMousePos.x * m_Properties.Sensitivity;
      float     xAngle = (float)deltaMousePos.y * m_Properties.Sensitivity;
      glm::mat4 mat =
        glm::rotate(glm::mat4(1.0f), xAngle * DEG_2_RAD, glm::vec3(1, 0, 0));

      glm::vec3 rot;

      rot.x = RAD_2_DEG * atan2f(mat[1][2], mat[2][2]);

      m_Properties.Transform.rotation.x += rot.x;
      m_Properties.Transform.rotation.x =
        std::min(std::max(m_Properties.Transform.rotation.x, -90.0f), 90.0f);

      mat =
        glm::rotate(glm::mat4(1.0f), yAngle * DEG_2_RAD, glm::vec3(0, 1, 0));
      rot.y = RAD_2_DEG *
              atan2f(-mat[0][2],
                     sqrtf(mat[1][2] * mat[1][2] + mat[2][2] * mat[2][2]));

      m_Properties.Transform.rotation.y += rot.y;
    }

    glm::vec3 movementVector = { (m_InputManager->GetKey(A) ? -1 : 0) +
                                   (m_InputManager->GetKey(D) ? 1 : 0),
                                 (m_InputManager->GetKey(Q) ? -1 : 0) +
                                   (m_InputManager->GetKey(E) ? 1 : 0),
                                 (m_InputManager->GetKey(W) ? -1 : 0) +
                                   (m_InputManager->GetKey(S) ? 1 : 0) };

    glm::mat4 rotMat =
      glm::rotate(glm::mat4(1.0f),
                  m_Properties.Transform.rotation.x * DEG_2_RAD,
                  glm::vec3(1.0f, 0.0f, 0.0f)) *
      glm::rotate(glm::mat4(1.0f),
                  m_Properties.Transform.rotation.y * DEG_2_RAD,
                  glm::vec3(0.0f, 1.0f, 0.0f));

    if (glm::length(movementVector) > 0)
    {
      movementVector = glm::normalize(movementVector);
      movementVector *= deltaTime * m_Properties.MovementSpeed *
                        (m_InputManager->GetKey(KEYCODE::LEFT_SHIFT) ? 2 : 1);

      glm::vec4 deltaVec4 =
        glm::inverse(rotMat) *
        glm::vec4(movementVector.x, movementVector.y, movementVector.z, 1.0f);

      m_Properties.Transform.position +=
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
    return m_Properties;
  }

  nlohmann::json
  Camera::Serialize() const
  {
    nlohmann::json j;
    j["transform"] = m_Properties.Transform.Serialize();
    j["fov"] = m_Properties.Fov;
    j["nearPlane"] = m_Properties.NearPlane;
    j["farPlane"] = m_Properties.FarPlane;
    j["aspectRatio"] = m_Properties.AspectRatio;
    j["sensitivity"] = m_Properties.Sensitivity;
    j["movementSpeed"] = m_Properties.MovementSpeed;
    return j;
  }
}