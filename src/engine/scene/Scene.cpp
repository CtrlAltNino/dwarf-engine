#include"Scene.h"
#include"../components/Components.h"
#include"../data structures/Entity.h"

// ========== Constructors ==========

Scene::Scene(const char* sceneName)
	: /*sceneCamera(glm::vec3(0, 2, 5), Quaternion::fromEulerAngles(25, 0, 0)),*/
		//clearColor(DEFAULT_CLEAR_COLOR),
		//lastMousePos(glm::vec2(-1)),
		sceneName(sceneName) {
	entt::entity entity = m_Registry.create();
}

Scene::~Scene(){
	// free entt stuff
}

// ========== Getters ==========

const char* Scene::getSceneName() {
	return sceneName;
}

/*Color Scene::getClearColor() {
	return clearColor;
}*/

// ========== Setters ==========

/*void Scene::setSceneName(const char* sceneName) {
	this->sceneName = sceneName;
}*/

/*void Scene::setClearColor(Color clearColor) {
	this->clearColor = clearColor;
}*/

// ========== Scene Functions ==========

Entity Scene::CreateEntity(const std::string& name){
	return CreateEntityWithUID(UID(), name);
}

Entity Scene::CreateEntityWithUID(UID uid, const std::string& name = std::string()){
	Entity entity = { m_Registry.create(), this};
	entity.AddComponent<IDComponent>(uid);
	entity.AddComponent<TransformComponent>();
	auto& tag = entity.AddComponent<TagComponent>();
	tag.Tag = name.empty() ? "Entity" : name;

	return entity;
}

void Scene::drawScene(Camera camera) {
	for (int i = 0; i < sceneObjects.size(); i++) {
		Object currentSceneObject = sceneObjects.at(i);
		if (currentSceneObject.active) {
			sceneObjects.at(i).render(camera.getViewMatrix(), camera.getProjectionMatrix());
		}
	}
	//sceneObjects.at(0).render(sceneCamera.getViewMatrix(), sceneCamera.getProjectionMatrix());
}