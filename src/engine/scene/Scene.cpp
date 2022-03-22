#include"Scene.h"
#include"../components/Components.h"

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

Entity Scene::CreateEntityWithUID(UID uid, const std::string& name){
	Entity entity = { m_Registry.create(), this};
	entity.AddComponent<IDComponent>(uid);
	entity.AddComponent<TransformComponent>();
	auto& tag = entity.AddComponent<TagComponent>(name);
	tag.Tag = name.empty() ? "Entity" : name;

	//entity.SetParent(rootEntity.GetHandle());

	return entity;
}

void Scene::drawScene(Camera camera) {
	/*for (int i = 0; i < sceneObjects.size(); i++) {
		Object currentSceneObject = sceneObjects.at(i);
		if (currentSceneObject.active) {
			sceneObjects.at(i).render(camera.getViewMatrix(), camera.getProjectionMatrix());
		}
	}*/

	glm::mat4x4 viewMatrix = camera.getViewMatrix();
	glm::mat4x4 projectionMatrix = camera.getProjectionMatrix();

	auto view = m_Registry.view<TransformComponent, MeshRendererComponent>();

	// use forward iterators and get only the components of interest
    for(auto entity: view) {
        auto &transform = view.get<TransformComponent>(entity);
        auto &renderer = view.get<MeshRendererComponent>(entity);

		for(auto mesh : renderer.meshes){
			Material mat = renderer.materials.at(mesh.materialIndex);
			mat.bind();
			mat.UpdateMVP(transform.getModelMatrix(), viewMatrix, projectionMatrix);
			mat.UpdateUniforms();

			if (mat.isTransparent) {
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			
			mesh.bind();
			glDrawElements(GL_TRIANGLES, mesh.indices2.size(), GL_UNSIGNED_INT, 0);
			mesh.unbind();

			glDisable(GL_BLEND);
			glDisable(GL_CULL_FACE);
		}

		
    }
	//sceneObjects.at(0).render(sceneCamera.getViewMatrix(), sceneCamera.getProjectionMatrix());
}