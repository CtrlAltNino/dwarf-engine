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
		glm::ivec2 deltaMousePos = InputManager::GetDeltaMousePos();

		if (deltaMousePos.length() > 0)
		{
			float yAngle = (float)deltaMousePos.x * m_sensitivity;
			float xAngle = (float)deltaMousePos.y * m_sensitivity;

			// TODO: Clamp rotation between -90 and 90 degrees
			// renderTexture->GetCamera()->transform.rotation.rotate(renderTexture->GetCamera()->transform.rotation.getQuaternion() * glm::vec3(0,1,0), yAngle);
			// renderTexture->GetCamera()->transform.rotation.rotate(glm::vec3(1,0,0), xAngle);
			// TransformComponent& tc = renderTexture->GetCamera()->transform;

			// tc.rotation.y += yAngle;
			// tc.rotation.x += xAngle;

			// glm::vec4 vecEuler = glm::vec4(tc.rotation.x, tc.rotation.y, tc.rotation.z, 1.0f);
			// vecEuler =
			glm::mat4 mat = glm::rotate(glm::mat4(1.0f), xAngle * DEG_2_RAD, glm::vec3(1, 0, 0));

			glm::vec3 rot;

			rot.x = RAD_2_DEG * atan2f(mat[1][2], mat[2][2]);
			// rot.y = RAD_2_DEG * atan2f(-mat[0][2], sqrtf(mat[1][2] * mat[1][2] + mat[2][2] * mat[2][2]));
			// rot.z = RAD_2_DEG * atan2f(mat[0][1], mat[0][0]);

			m_Transform->rotation.x += rot.x;
			m_Transform->rotation.x = std::min(std::max(m_Transform->rotation.x, -90.0f), 90.0f);

			mat = glm::rotate(glm::mat4(1.0f), yAngle * DEG_2_RAD, glm::vec3(0, 1, 0));
			// rot.x = RAD_2_DEG * atan2f(mat[1][2], mat[2][2]);
			rot.y = RAD_2_DEG * atan2f(-mat[0][2], sqrtf(mat[1][2] * mat[1][2] + mat[2][2] * mat[2][2]));
			// rot.z = RAD_2_DEG * atan2f(mat[0][1], mat[0][0]);

			m_Transform->rotation.y += rot.y;

			// std::cout << "Camera Euler Angles: x=" << tc.rotation.x << " y=" << tc.rotation.y << " z=" << tc.rotation.z << std::endl;
			/*glm::quat q = renderTexture->GetCamera()->transform.rotation.getQuaternion();

			float yaw = RAD_2_DEG * atan2(2.0f*(q.y*q.z + q.w*q.x), q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z);
			float pitch = RAD_2_DEG * asin(-2.0f*(q.x*q.z - q.w*q.y));
			float roll = RAD_2_DEG * atan2(2.0f*(q.x*q.y + q.w*q.z), q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z);

			glm::vec3 clampedRot = renderTexture->GetCamera()->transform.getEulerAngles();*/
			// std::cout << "yaw=" << yaw << " pitch=" << pitch << " roll=" << roll << std::endl;
			// std::cout << "Camera Euler Angles: x=" << clampedRot.x << " y=" << clampedRot.y << " z=" << clampedRot.z << std::endl;
			// clampedRot.x = std::max(std::min(90.0f, clampedRot.x), -90.0f);
			// std::cout << "\tCamera Euler Angles: x=" << clampedRot.x << " y=" << clampedRot.y << " z=" << clampedRot.z << std::endl;

			// renderTexture->GetCamera()->transform.setRotation(renderTexture->GetCamera()->transform.rotation.getEulerAngles());
		}

		glm::vec3 movementVector = {(InputManager::GetKey(KEYCODE::A) ? -1 : 0) + (InputManager::GetKey(KEYCODE::D) ? 1 : 0),
									(InputManager::GetKey(KEYCODE::Q) ? -1 : 0) + (InputManager::GetKey(KEYCODE::E) ? 1 : 0),
									(InputManager::GetKey(KEYCODE::W) ? -1 : 0) + (InputManager::GetKey(KEYCODE::S) ? 1 : 0)};

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
}