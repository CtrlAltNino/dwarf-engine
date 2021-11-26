#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include<glad/glad.h>
#include<stdlib.h>
#include<string>
#include<vector>
#include"Object.h"
#include"Camera.h"

#define DEFAULT_CLEAR_COLOR Color(0.1f, 0.1f, 0.1f, 1.0f)

class Scene {
private:
	const char* sceneName;
	Color clearColor;
public:
	Camera sceneCamera;
	std::vector<Object> sceneObjects;
	glm::vec2 deltaMousePos;
	glm::vec2 lastMousePos;
	// ========== Constructors ==========
	
	Scene(const char* sceneName);
	
	// ========== Getters ==========

	const char* getSceneName();
	Color getClearColor();
	
	// ========== Setters ==========

	void setSceneName(const char* sceneName);
	void setClearColor(Color clearColor);
	
	// ========== Scene Functions ==========
	
	void drawScene();
};

#endif