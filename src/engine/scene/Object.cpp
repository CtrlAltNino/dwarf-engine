#include"Object.h"

// ========== Constructors ==========

Object::Object(const char* objectName, Transform* transform, Renderer* renderer) : transform(*(Transform*)transform), renderer(*(Renderer*)renderer) {
	this->objectName = objectName;
}

// ========== Getters ==========

const char* Object::getObjectName() {
	return objectName;
}

// ========== Setters ==========

void Object::setObjectName(const char* objectName) {
	this->objectName = objectName;
}

void Object::setTransform(Transform* transform) {
	this->transform = *(Transform*)transform;
}

void Object::setRenderer(Renderer* renderer) {
	this->renderer = *(Renderer*)renderer;
}

// ========== Object Functions ==========

void Object::render(glm::mat4x4 viewMatrix, glm::mat4x4 projectionMatrix) {
	this->renderer.render(transform.getModelMatrix(), viewMatrix, projectionMatrix);
}