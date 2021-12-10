#ifndef TRANSFORM_CLASS_H
#define TRANSFORM_CLASS_H

#include<glad/glad.h>
#include<stdlib.h>
#include<string>
#include<vector>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>
#include<iostream>
#include"../data structures/Quaternion.h"

class Transform {
private:
	glm::vec3 position, scale;
	Quaternion rotation;
public:
	// ========== Constructors ==========
	
	Transform(glm::vec3 position);
	Transform(glm::vec3 position, Quaternion rotation);
	Transform(glm::vec3 position, glm::vec3 eulerAngles);

	// ========== Getters ==========
	
	glm::vec3 getPosition();
	Quaternion getRotation();
	glm::vec3 getScale();
	glm::vec3 getForward();
	glm::vec3 getUp();
	glm::vec3 getRight();
	glm::mat4x4 getModelMatrix();

	// ========== Setters ==========
	
	void setPosition(glm::vec3);
	void setRotation(Quaternion rotation);
	void setRotation(glm::vec3 eulerAngles);
	void setScale(glm::vec3 scale);

	// ========== Transform Operations ==========
	
	void translate(glm::vec3 translationVector);
	void rotate(glm::vec3 eulerAngles);
	void rotate(glm::vec3 rotationAxis, float angle);
	void rotateGlobally(glm::vec3 eulerAngles);
	void lookAt();
};

#endif