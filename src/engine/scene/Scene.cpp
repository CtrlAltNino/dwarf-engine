#include"Scene.h"
#include"../components/Components.h"

// ========== Constructors ==========

Scene::Scene(const char* sceneName)
	: sceneName(sceneName) {
	// entt::entity entity = m_Registry.create();
}

Scene::~Scene(){ }

// ========== Getters ==========

const char* Scene::getSceneName() {
	return sceneName;
}

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

	entity.SetParent(rootEntity.GetHandle());

	return entity;
}

void Scene::drawScene(Camera camera) {

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
}

void Scene::selectEntity(Entity entity) {
	clearSelection();
	addEntityToSelection(entity);
}

void Scene::addEntityToSelection(Entity entity){
	std::string index = GetTreeIndex(entity);

	std::vector<Entity>::iterator cursor = selectedEntities.begin();
	std::string cursorIndex;

	while((cursor != selectedEntities.end()) && ((cursorIndex = GetTreeIndex(*cursor)) < index)){
		cursor++;
	}

	if(cursor == selectedEntities.end()){
		selectedEntities.push_back(entity);
	}else{
		selectedEntities.insert(cursor, entity);
	}
}

std::string Scene::GetTreeIndex(Entity entity){
	std::string index = "";
	Entity cursor = entity;

	while(cursor.GetHandle() != rootEntity.GetHandle()){
		index = std::to_string(cursor.GetChildIndex()) + index;
		cursor = Entity(cursor.GetParent(), this);
	}

	return index;
}

void Scene::removeEntityFromSelection(Entity entity){
	selectedEntities.erase(std::find(selectedEntities.begin(), selectedEntities.end(), entity));
}

boolean Scene::isEntitySelected(Entity entity){
	return std::find(selectedEntities.begin(), selectedEntities.end(), entity) != selectedEntities.end();
}

void Scene::clearSelection(){
	selectedEntities.clear();
}