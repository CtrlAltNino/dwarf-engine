#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include"Transform.h"
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>
#include"../data structures/Quaternion.h"

class Camera {
private:
	float fov, nearPlane, farPlane, aspectRatio;
public:
	Transform transform;
	// ========== Constructors ==========
	
	Camera(glm::vec3 position, Quaternion rotation, float fov, float nearPlane, float farPlane, float aspectRatio);
	Camera(glm::vec3 position, Quaternion rotation);

	// ========== Getters ==========
	
	float getFov();
	glm::vec2 getRenderPlaneParameters();
	float getAspectRatio();
	glm::mat4x4 getViewMatrix();
	glm::mat4x4 getProjectionMatrix();
	
	// ========== Setters ==========
	
	void setTransform(Transform* transform);
	void setFov(float fov);
	void setRenderPlaneParameters(glm::vec2 planeParameters);
	void setAspectRatio(float aspectRatio);
	
	// ========== Camera Functions ==========
	
};

#endif