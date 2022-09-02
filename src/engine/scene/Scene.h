#pragma once
#include "../../utilities/dpch.h"

#include"../data structures/Entity.h"
#include"EntityProvider.h"
#include"../components/Components.h"

#include<glad/glad.h>
#include<entt/entt.hpp>

#include"Object.h"
#include"Camera.h"

class Scene : public EntityProvider {
public:
	Entity rootEntity = CreateEntity("root");
	Entity selectedEntity = Entity(entt::null, nullptr);
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
private:
	const char* sceneName;
	friend class Entity;
};