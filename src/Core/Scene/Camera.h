#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>

#include "Core/Base.h"
#include "Core/Scene/SceneComponents.h"

namespace Dwarf
{

	/// @brief Class representing a camera.
	class Camera
	{
	private:
		/// @brief The transform of the camera, representing its position and rotation.
		Ref<TransformComponent> m_Transform;

		/// @brief The camera's field of view expressed in degrees.
		float m_Fov = 90.0f;

		/// @brief The near plane distance.
		float m_NearPlane = 0.1f;

		/// @brief The far plane distance.
		float m_FarPlane = 1000.0f;

		/// @brief The aspect ratio of the camera.
		float m_AspectRatio = 16.0f / 9.0f;

		float m_sensitivity = 0.15f;

		float m_MovementSpeed = 4.0f;

	public:
		// ========== Constructors ==========

		Camera();
		Camera(glm::vec3 position, glm::vec3 rotation);
		Camera(glm::vec3 position, glm::vec3 rotation, float fov, float nearPlane, float farPlane, float aspectRatio);

		// ========== Getters ==========

		/// @brief Returns the field of view.
		/// @return Fov of the camera.
		float GetFov() const;

		/// @brief Returns the near and far plane distances.
		/// @return The near and far plane distances as a 2D vector.
		glm::vec2 GetRenderPlaneParameters() const;

		/// @brief Returns the aspect ratio of the camera.
		/// @return Aspect ratio.
		float GetAspectRatio() const;

		/// @brief Returns the view matrix of the camera.
		/// @return 4x4 view matrix.
		glm::mat4x4 GetViewMatrix() const;

		/// @brief Returns the projection matrix of the camera.
		/// @return 4x4 projection matrix.
		glm::mat4x4 GetProjectionMatrix() const;

		Ref<TransformComponent> GetTransform() const;

		// ========== Setters ==========

		/// @brief Sets the transform of the camera.
		/// @param transform Pointer to a transform.
		void SetTransform(Ref<TransformComponent> transform);

		/// @brief Sets the field of view of the camera.
		/// @param fov Field of view expressed in degrees.
		void SetFov(float fov);

		/// @brief Sets the near and far planes parameters.
		/// @param planeParameters 2D vector containing the near plane information in the x, and the far plane information in the y component.
		void SetRenderPlaneParameters(glm::vec2 planeParameters);

		/// @brief Sets the aspect ratio of the camera.
		/// @param aspectRatio Aspect ratio value.
		void SetAspectRatio(float aspectRatio);

		// ========== Camera Functions ==========
		void OnUpdate(double deltaTime) const;

		glm::vec3 ScreenToWorld(glm::vec2 const &screenPosition, glm::vec2 const &viewport) const;
	};
}