#ifndef OBJECT_CLASS_H
#define OBJECT_CLASS_H

#include<glad/glad.h>
#include<glm/glm.hpp>
#include"Transform.h"
#include"Renderer.h"
#include"../data structures/Mesh.h"

class Object {
private:
	const char* objectName;
public:
	bool active = true;
	Transform transform;
	Renderer renderer;
	// ========== Constructors ==========
	
	Object(const char* objectName, Transform* transform, Renderer* renderer);

	// ========== Getters ==========
	
	const char* getObjectName();

	// ========== Setters ==========
	
	void setObjectName(const char* objectName);
	void setTransform(Transform* transform);
	void setRenderer(Renderer* renderer);

	// ========== Object Functions ==========
	
	void render(glm::mat4x4 viewMatrix, glm::mat4x4 projectionMatrix);
};

#endif