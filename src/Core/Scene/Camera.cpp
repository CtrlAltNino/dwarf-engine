#include "dpch.h"
#include "Core/Scene/Camera.h"
#include "Input/InputManager.h"

namespace Dwarf
{

	// ========== Makros ==========

#define DEFAULT_CAMERA_POSITION glm::vec3(0, 2, 15)
#define DEFAULT_CAMERA_ROTATION glm::vec3(30, 0, 0)

	// ========== Constructors ==========
	Camera::Camera()
		: m_Transform(CreateRef<TransformComponent>(DEFAULT_CAMERA_POSITION, DEFAULT_CAMERA_ROTATION)) {}

	Camera::Camera(glm::vec3 position, glm::vec3 rotation)
		: m_Transform(CreateRef<TransformComponent>(position, rotation)) {}

	Camera::Camera(glm::vec3 position, glm::vec3 rotation, float fov, float nearPlane, float farPlane, float aspectRatio)
		: m_Transform(CreateRef<TransformComponent>(position, rotation)),
		  m_Fov(fov),
		  m_NearPlane(nearPlane),
		  m_FarPlane(farPlane),
		  m_AspectRatio(aspectRatio) {}

	// ========== Getters ==========

	float Camera::GetFov() const
	{
		return m_Fov;
	}

	glm::vec2 Camera::GetRenderPlaneParameters() const
	{
		return glm::vec2(m_NearPlane, m_FarPlane);
	}

	float Camera::GetAspectRatio() const
	{
		return m_AspectRatio;
	}

	glm::mat4x4 Camera::GetViewMatrix() const
	{
		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), m_Transform->rotation.x * DEG_2_RAD, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), m_Transform->rotation.y * DEG_2_RAD, glm::vec3(0.0f, 1.0f, 0.0f));

		return rot * glm::translate(glm::mat4(1.0f), -m_Transform->getPosition());
	}

	glm::mat4x4 Camera::GetProjectionMatrix() const
	{
		return glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_NearPlane, m_FarPlane);
	}

	Ref<TransformComponent> Camera::GetTransform() const
	{
		return m_Transform;
	}

	// ========== Setters ==========

	void Camera::SetTransform(Ref<TransformComponent> transform)
	{
		m_Transform = transform;
	}

	void Camera::SetFov(float fov)
	{
		m_Fov = fov;
	}

	void Camera::SetRenderPlaneParameters(glm::vec2 planeParameters)
	{
		m_NearPlane = planeParameters.x;
		m_FarPlane = planeParameters.y;
	}

	void Camera::SetAspectRatio(float aspectRatio)
	{
		m_AspectRatio = aspectRatio;
	}

	// ========== Camera Functions ==========
	void Camera::OnUpdate(double deltaTime) const
	{
		using enum KEYCODE;
		glm::ivec2 deltaMousePos = InputManager::GetDeltaMousePos();

		if (deltaMousePos.length() > 0)
		{
			float yAngle = (float)deltaMousePos.x * m_sensitivity;
			float xAngle = (float)deltaMousePos.y * m_sensitivity;
			glm::mat4 mat = glm::rotate(glm::mat4(1.0f), xAngle * DEG_2_RAD, glm::vec3(1, 0, 0));

			glm::vec3 rot;

			rot.x = RAD_2_DEG * atan2f(mat[1][2], mat[2][2]);

			m_Transform->rotation.x += rot.x;
			m_Transform->rotation.x = std::min(std::max(m_Transform->rotation.x, -90.0f), 90.0f);

			mat = glm::rotate(glm::mat4(1.0f), yAngle * DEG_2_RAD, glm::vec3(0, 1, 0));
			rot.y = RAD_2_DEG * atan2f(-mat[0][2], sqrtf(mat[1][2] * mat[1][2] + mat[2][2] * mat[2][2]));

			m_Transform->rotation.y += rot.y;
		}

		glm::vec3 movementVector = {(InputManager::GetKey(A) ? -1 : 0) + (InputManager::GetKey(D) ? 1 : 0),
									(InputManager::GetKey(Q) ? -1 : 0) + (InputManager::GetKey(E) ? 1 : 0),
									(InputManager::GetKey(W) ? -1 : 0) + (InputManager::GetKey(S) ? 1 : 0)};

		glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), m_Transform->rotation.x * DEG_2_RAD, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), m_Transform->rotation.y * DEG_2_RAD, glm::vec3(0.0f, 1.0f, 0.0f));

		if (glm::length(movementVector) > 0)
		{
			movementVector = glm::normalize(movementVector);
			movementVector *= deltaTime * m_MovementSpeed * (InputManager::GetKey(KEYCODE::LEFT_SHIFT) ? 2 : 1);

			glm::vec4 deltaVec4 = glm::inverse(rotMat) * glm::vec4(movementVector.x, movementVector.y, movementVector.z, 1.0f);

			m_Transform->position += glm::vec3(deltaVec4.x, deltaVec4.y, deltaVec4.z);
		}

		deltaMousePos = glm::vec2(0);
	}

	glm::vec3 Camera::ScreenToWorld(glm::vec2 const &screenPos, glm::vec2 const &viewport) const
	{
		float x = (2.0f * screenPos.x) / viewport.x - 1.0f;
		float y = 1.0f - (2.0f * screenPos.y) / viewport.y;
		float z = 1.0f;
		glm::vec3 ray_nds = glm::vec3(x, y, z);
		glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
		glm::vec4 ray_eye = glm::inverse(GetProjectionMatrix()) * ray_clip;
		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
		glm::vec3 ray_wor = glm::vec3(glm::inverse(GetViewMatrix()) * ray_eye);
		ray_wor = glm::normalize(ray_wor);
		return ray_wor;
	}
}