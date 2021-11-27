#include"Scene.h"

// ========== Constructors ==========

Scene::Scene(const char* sceneName)
	: sceneCamera(glm::vec3(0, 2, 5), Quaternion::fromEulerAngles(25, 0, 0)),
		clearColor(DEFAULT_CLEAR_COLOR),
		lastMousePos(glm::vec2(-1)) {
	this->sceneName = sceneName;
}

// ========== Getters ==========

const char* Scene::getSceneName() {
	return sceneName;
}

Color Scene::getClearColor() {
	return clearColor;
}

// ========== Setters ==========

void Scene::setSceneName(const char* sceneName) {
	this->sceneName = sceneName;
}

void Scene::setClearColor(Color clearColor) {
	this->clearColor = clearColor;
}

// ========== Scene Functions ==========

void Scene::drawScene() {
	for (int i = 0; i < sceneObjects.size(); i++) {
		Object currentSceneObject = sceneObjects.at(i);
		if (currentSceneObject.active) {
			sceneObjects.at(i).render(sceneCamera.getViewMatrix(), sceneCamera.getProjectionMatrix());
		}
	}
	//sceneObjects.at(0).render(sceneCamera.getViewMatrix(), sceneCamera.getProjectionMatrix());
}