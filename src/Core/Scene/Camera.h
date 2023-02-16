#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>

#include "Core/Scene/Components.h"

namespace Dwarf {

	/// @brief Class representing a camera.
	class Camera {
	private:
		/// @brief The camera's field of view expressed in degrees.
		float fov;

		/// @brief The near plane distance.
		float nearPlane;

		/// @brief The far plane distance.
		float farPlane;

		/// @brief The aspect ratio of the camera.
		float aspectRatio;
	public:
		/// @brief The transform of the camera, representing its position and rotation.
		TransformComponent transform;
		// ========== Constructors ==========
		
		Camera(glm::vec3 position, glm::vec3 rotation, float fov, float nearPlane, float farPlane, float aspectRatio);
		Camera(glm::vec3 position, glm::vec3 rotation);

		// ========== Getters ==========
		
		/// @brief Returns the field of view.
		/// @return Fov of the camera.
		float getFov();

		/// @brief Returns the near and far plane distances.
		/// @return The near and far plane distances as a 2D vector.
		glm::vec2 getRenderPlaneParameters();

		/// @brief Returns the aspect ratio of the camera.
		/// @return Aspect ratio.
		float getAspectRatio();

		/// @brief Returns the view matrix of the camera.
		/// @return 4x4 view matrix.
		glm::mat4x4 getViewMatrix();

		/// @brief Returns the projection matrix of the camera.
		/// @return 4x4 projection matrix.
		glm::mat4x4 getProjectionMatrix();
		
		// ========== Setters ==========
		
		/// @brief Sets the transform of the camera.
		/// @param transform Pointer to a transform.
		void setTransform(TransformComponent* transform);

		/// @brief Sets the field of view of the camera.
		/// @param fov Field of view expressed in degrees.
		void setFov(float fov);

		/// @brief Sets the near and far planes parameters.
		/// @param planeParameters 2D vector containing the near plane information in the x, and the far plane information in the y component.
		void setRenderPlaneParameters(glm::vec2 planeParameters);

		/// @brief Sets the aspect ratio of the camera.
		/// @param aspectRatio Aspect ratio value.
		void setAspectRatio(float aspectRatio);
		
		// ========== Camera Functions ==========
		
	};
}