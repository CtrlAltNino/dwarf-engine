#include"Transform.h"

// ========== Constructors ==========

Transform::Transform(glm::vec3 position) {
	this->position = position;
	this->rotation = glm::quat({0,0,0});
	this->scale = glm::vec3(1, 1, 1);
}

Transform::Transform(glm::vec3 position, glm::quat rotation) {
	this->position = position;
	this->rotation = rotation;
	this->scale = glm::vec3(1, 1, 1);
}

Transform::Transform(glm::vec3 position, glm::vec3 eulerAngles) {
	this->position = position;
	this->rotation = glm::quat(eulerAngles);
	this->scale = glm::vec3(1, 1, 1);
}

// ========== Getters ==========

glm::vec3 Transform::getPosition() {
	return this->position;
}

glm::quat Transform::getRotation() {
	return this->rotation;
}

glm::vec3 Transform::getScale() {
	return this->scale;
}

glm::vec3 Transform::getForward() {
	return rotation * glm::vec3(0, 0, -1);
}

glm::vec3 Transform::getUp() {
	return rotation * glm::vec3(0, 1, 0);
}

glm::vec3 Transform::getRight() {
	return rotation * glm::vec3(1, 0, 0);
}

glm::mat4x4 Transform::getModelMatrix() {
	// TODO Fix scale and rotation part of the model matrix
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	//glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation.GetAngle(), rotation.GetAxis());
	glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
	//std::cout << "Angle: " << rotation.GetAngle() << " | Axis: [" << rotation.GetAxis().x << ", " << rotation.GetAxis().y << ", " << rotation.GetAxis().z << "]" << std::endl;
	//std::cout << "Quaterion: X = " << rotation.x << " | Y = " << rotation.y << " | Z = " << rotation.z << " | R = " << rotation.r << std::endl;
	//std::cout << "Euler Angles: X = " << rotation.GetEulerAngles().x << " | Y = " << rotation.GetEulerAngles().y << " | Z = " << rotation.GetEulerAngles().z << std::endl;
	float angle = glm::radians(15.0f);
	glm::mat4 yeet = {
		cos(angle), 0, -sin(angle), 0,
		0, 1, 0, 0,
		sin(angle), 0, cos(angle), 0,
		0, 0, 0, 1
	};

	return translationMatrix * rotationMatrix * scaleMatrix;
}

// ========== Setters ==========

void Transform::setPosition(glm::vec3 position) {
	this->position = position;
}

void Transform::setRotation(glm::quat rotation) {
	this->rotation = rotation;
}

void Transform::setRotation(glm::vec3 eulerAngles) {
	this->rotation = glm::quat(eulerAngles);
}

void Transform::setScale(glm::vec3 scale) {
	this->scale = scale;
}

// ========== Transform Operations ==========

void Transform::reset(){
	setPosition(glm::vec3(0));
	setRotation(glm::vec3(0));
	setScale(glm::vec3(1));
}

void Transform::translate(glm::vec3 translationVector) {
	this->position += translationVector;
}

void Transform::rotate(glm::vec3 eulerAngles) {
	rotate(glm::vec3(1,0,0), eulerAngles.x);
	rotate(glm::vec3(0,0,-1), eulerAngles.z);
	rotate(glm::vec3(0,1,0), eulerAngles.y);
}

void Transform::rotate(glm::vec3 rotationAxis, float angle) {
	this->rotation = glm::rotate(this->rotation, angle, rotationAxis);
}

void Transform::rotateGlobally(glm::vec3 eulerAngles) {
	// TODO IMPLEMENT
}

void Transform::lookAt() {
	// TODO implement
}