#pragma once

#include "../../utilities/dpch.h"

#include<glad/glad.h>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

//#include"../data structures/Quaternion.h"

class Transform {
private:
	glm::vec3 position, scale;
	glm::quat rotation;
public:
	// ========== Constructors ==========
	
	Transform(glm::vec3 position);
	Transform(glm::vec3 position, glm::quat rotation);
	Transform(glm::vec3 position, glm::vec3 eulerAngles);

	// ========== Getters ==========
	
	glm::vec3 getPosition();
	glm::quat getRotation();
	glm::vec3 getScale();
	glm::vec3 getForward();
	glm::vec3 getUp();
	glm::vec3 getRight();
	glm::mat4x4 getModelMatrix();

	// ========== Setters ==========
	
	void setPosition(glm::vec3);
	void setRotation(glm::quat rotation);
	void setRotation(glm::vec3 eulerAngles);
	void setScale(glm::vec3 scale);

	// ========== Transform Operations ==========
	
	void reset();
	void translate(glm::vec3 translationVector);
	void rotate(glm::vec3 eulerAngles);
	void rotate(glm::vec3 rotationAxis, float angle);
	void rotateGlobally(glm::vec3 eulerAngles);
	void lookAt();
};