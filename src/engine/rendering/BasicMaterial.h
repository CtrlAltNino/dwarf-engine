#pragma once

#include "../../utilities/dpch.h"

#include<glad/glad.h>
#include<glm/glm.hpp>

#include"Shader.h"
#include"Material.h"
#include"../data structures/Color.h"
#include"../data structures/Texture.h"

/// @brief Structure containing settings of a material.
struct MaterialSettings {
	/// @brief Shininess of the material.
	float shininess = 4;

	/// @brief Flag indicating if an object should be rendered as transparent.
	bool isTransparent = false;

	/// @brief Flag indicating if faces should be culled.
	bool cullFaces = true;

	/// @brief OpenGL enum defining the cull mode.
	GLenum cullMode = GL_BACK;

	/// @brief Placeholder fog start value.
	float fogStart = 10;

	/// @brief Placeholder fog end value.
	float fogEnd = 60;

	/// @brief Placeholder fog color.
	glm::vec4 fogColor = glm::vec4(0.1, 0.1, 0.1, 1);
};

/// @brief 
class BasicMaterial{
private:
	/// @brief Color tint.
	Color color = Color(1,1,1,1);

	/// @brief Associated shader.
	Shader shader;

	/// @brief Base color texture.
	Texture* albedoMap = nullptr;

	/// @brief Texture containing specularity.
	Texture* specularMap = nullptr;

	/// @brief Texture containing normal overrides.
	Texture* normalMap = nullptr;
public:
	/// @brief Settings of the material.
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