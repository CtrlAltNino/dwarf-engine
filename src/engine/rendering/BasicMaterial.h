#pragma once

#include "../../utilities/dpch.h"

#include<glad/glad.h>
#include<glm/glm.hpp>

#include"Shader.h"
#include"Material.h"
#include"../data structures/Color.h"
#include"../data structures/Texture.h"

struct MaterialSettings {
	float shininess = 4;
	bool isTransparent = false;
	bool cullFaces = true;
	GLenum cullMode = GL_BACK;
	float fogStart = 10;
	float fogEnd = 60;
	glm::vec4 fogColor = glm::vec4(0.1, 0.1, 0.1, 1);
};

class BasicMaterial{
private:
	Color color = Color(1,1,1,1);
	Shader shader;
	Texture* albedoMap = nullptr;
	Texture* specularMap = nullptr;
	Texture* normalMap = nullptr;
public:
	MaterialSettings settings;
	// ========== Constructors ==========
	
	// BasicMaterial(const char* shaderName);

	// ========== Getters ==========
	
	Color getColor();
	Shader getShader();
	Texture getAlbedoMap();
	Texture getSpecularMap();
	Texture getNormalMap();
	
	// ========== Setters ==========
	
	void setColor(Color color);
	void setColor(float r, float g, float b);
	void setColor(float r, float g, float b, float a);
	void setShader(Shader shader);
	void setAlbedoMap(const char* texturePath);
	void setAlbedoMap(Texture texture);
	void setSpecularMap(const char* texturePath);
	void setSpecularMap(Texture texture);
	void setNormalMap(const char* texturePath);
	void setNormalMap(Texture texture);
	
	// ========== Material Functions ==========
	
	void bind();
	void free();
	void updateShaderParameters(glm::mat4x4 modelMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 projectionMatrix);
};