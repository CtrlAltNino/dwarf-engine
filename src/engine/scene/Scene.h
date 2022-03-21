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

#define DEFAULT_CLEAR_COLOR Color(0.1f, 0.1f, 0.1f, 1.0f)

class Entity;
class Scene {
public:
	entt::registry m_Registry;
	//Camera sceneCamera;
	std::vector<Object> sceneObjects;
	//glm::vec2 deltaMousePos;
	//glm::vec2 lastMousePos;
	// ========== Constructors ==========
	
	Scene(const char* sceneName);
	~Scene();
	
	// ========== Getters ==========

	const char* getSceneName();
	//Color getClearColor();
	
	// ========== Setters ==========

	//void setSceneName(const char* sceneName);
	//void setClearColor(Color clearColor);
	
	// ========== Scene Functions ==========
	
	Entity CreateEntity(const std::string& name = std::string());
	Entity CreateEntityWithUID(UID uid, const std::string& name = std::string());
	void drawScene(Camera camera);
private:
	const char* sceneName;
	//Color clearColor;
	friend class Entity;
};