#include"Camera.h"

// ========== Makros ==========

#define DEFAULT_CAMERA_POSITION glm::vec3(2, 2, 0)
#define DEFAULT_CAMERA_ROTATION Quaternion::FromEulerAngles(30.0f, 0, 0)
#define DEFAULT_CAMERA_FOV 75.0f
#define DEFAULT_CAMERA_NEARPLANE 0.1f
#define DEFAULT_CAMERA_FARPLANE 1000.0f
#define DEFAULT_CAMERA_ASPECT_RATIO (16.0f / 9.0f)

// ========== Constructors ==========

Camera::Camera(glm::vec3 position, Quaternion rotation, float fov, float nearPlane, float farPlane, float aspectRatio)
	: transform(position, rotation){
	//this->transform = new Transform(position, rotation);
	this->fov = fov;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	this->aspectRatio = aspectRatio;
}

Camera::Camera(glm::vec3 position, Quaternion rotation)
	: transform(position, rotation) {
	//this->transform = new Transform(position, rotation);
	this->fov = DEFAULT_CAMERA_FOV;
	this->nearPlane = DEFAULT_CAMERA_NEARPLANE;
	this->farPlane = DEFAULT_CAMERA_FARPLANE;
	this->aspectRatio = DEFAULT_CAMERA_ASPECT_RATIO;
}

// ========== Getters ==========

float Camera::getFov() {
	return fov;
}

glm::vec2 Camera::getRenderPlaneParameters() {
	return glm::vec2(nearPlane, farPlane);
}

float Camera::getAspectRatio() {
	return aspectRatio;
}

glm::mat4x4 Camera::getViewMatrix() {
	return transform.getRotation().getMatrix() * glm::translate(glm::mat4(1.0f), -transform.getPosition());
}

glm::mat4x4 Camera::getProjectionMatrix() {
	return glm::perspective(glm::radians(this->fov), this->aspectRatio, this->nearPlane, this->farPlane);
}

// ========== Setters ==========

void Camera::setTransform(Transform* transform) {
	this->transform = *(Transform*)transform;
}

void Camera::setFov(float fov) {
	this->fov = fov;
}

void Camera::setRenderPlaneParameters(glm::vec2 planeParameters) {
	this->nearPlane = planeParameters.x;
	this->farPlane = planeParameters.y;
}

void Camera::setAspectRatio(float aspectRatio) {
	this->aspectRatio = aspectRatio;
}

// ========== Camera Functions ==========