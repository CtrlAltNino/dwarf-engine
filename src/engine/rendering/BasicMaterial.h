#ifndef BASIC_MATERIAL_CLASS_H
#define BASIC_MATERIAL_CLASS_H

#include<glad/glad.h>
#include"../data/Color.h"
#include"Shader.h"
#include"../data/Texture.h"
#include"Material.h"
#include<glm/glm.hpp>
#include<regex>

struct MaterialSettings {
	float shininess = 4;
	bool isTransparent = false;
	bool cullFaces = true;
	GLenum cullMode = GL_BACK;
};

class BasicMaterial{
private:
	Color color = Color(1,1,1,1);
	Shader shader;
	Texture* albedoMap;
	Texture* specularMap;
	Texture* normalMap;
public:
	MaterialSettings settings;
	// ========== Constructors ==========
	
	BasicMaterial(const char* shaderName);

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

#endif