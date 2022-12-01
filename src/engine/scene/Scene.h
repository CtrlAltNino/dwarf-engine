#pragma once
#include "../../utilities/dpch.h"

#include"../data structures/Entity.h"
#include"EntityProvider.h"
#include"../components/Components.h"

#include<glad/glad.h>

#include"Object.h"
#include"Camera.h"

class Scene : public EntityProvider {
public:
	Entity rootEntity = CreateEntity("root");
	std::vector<Entity> selectedEntities;
	// ========== Constructors ==========
	
	Scene(const char* sceneName);
	~Scene();
	
	// ========== Getters ==========

	const char* getSceneName();
	
	// ========== Scene Functions ==========
	
	Entity CreateEntity(const std::string& name = std::string());
	Entity CreateEntityWithUID(UID uid, const std::string& name);
	void drawScene(Camera camera);
	void selectEntity(Entity entity);
	void addEntityToSelection(Entity entity);
	void removeEntityFromSelection(Entity entity);
	boolean isEntitySelected(Entity entity);
	void clearSelection();
private:
	const char* sceneName;
	friend class Entity;
	std::string GetTreeIndex(Entity entity);
	glm::mat4 GetFullModelMatrix(TransformComponent transform);
};