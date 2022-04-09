#pragma once

//#include"../editor/UID.h"
#include"../components/Components.h"
#include"Object.h"
#include"Camera.h"

#include<glad/glad.h>
#include<stdlib.h>
#include<string>
#include<vector>
#include<entt/entt.hpp>
#include"../data structures/Entity.h"
#include"EntityProvider.h"


class Scene : public EntityProvider {
public:
	std::vector<Object> sceneObjects;
	Entity rootEntity = Entity(entt::null, nullptr);
	// ========== Constructors ==========
	
	Scene(const char* sceneName);
	~Scene();
	
	// ========== Getters ==========

	const char* getSceneName();
	
	// ========== Scene Functions ==========
	
	Entity CreateEntity(const std::string& name = std::string());
	Entity CreateEntityWithUID(UID uid, const std::string& name);
	void drawScene(Camera camera);
private:
	const char* sceneName;
	friend class Entity;
};