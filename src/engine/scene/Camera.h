#pragma once

#include "../../utilities/dpch.h"
#include"../components/Components.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>

//#include"../data structures/Quaternion.h"
//#include"Transform.h"

class Camera {
private:
	float fov, nearPlane, farPlane, aspectRatio;
public:
	TransformComponent transform;
	// ========== Constructors ==========
	
	Camera(glm::vec3 position, glm::vec3 rotation, float fov, float nearPlane, float farPlane, float aspectRatio);
	Camera(glm::vec3 position, glm::vec3 rotation);

	// ========== Getters ==========
	
	float getFov();
	glm::vec2 getRenderPlaneParameters();
	float getAspectRatio();
	glm::mat4x4 getViewMatrix();
	glm::mat4x4 getProjectionMatrix();
	
	// ========== Setters ==========
	
	void setTransform(TransformComponent* transform);
	void setFov(float fov);
	void setRenderPlaneParameters(glm::vec2 planeParameters);
	void setAspectRatio(float aspectRatio);
	
	// ========== Camera Functions ==========
	
};