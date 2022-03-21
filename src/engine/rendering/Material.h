#pragma once

#include<glad/glad.h>
#include"../data structures/Color.h"
#include"Shader.h"
#include"../data structures/Texture.h"
#include"Shader.h"
#include<iostream>
#include<map>
#include<glm/glm.hpp>

class Material {
	private:
		std::map<std::string, Texture> textures = std::map<std::string, Texture>();
		std::map<std::string, bool> boolUniforms;
		std::map<std::string, int> integerUniforms;
		std::map<std::string, float> floatUniforms;
		std::map<std::string, glm::vec2> vector2Uniforms;
		std::map<std::string, glm::vec3> vector3Uniforms;
		std::map<std::string, glm::vec4> vector4Uniforms;
	public:
		Shader shader;
		bool isTransparent = false;
		Material() = default;
		Material(Shader* shader);
		~Material();

		void bind();
		void UpdateUniforms();
		void UpdateMVP(glm::mat4x4 modelMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 projectionMatrix);

		void UpdateShaderInput(const char* uniformName, Texture* val);
		void UpdateShaderInput(const char* uniformName, bool val);
		void UpdateShaderInput(const char* uniformName, int val);
		void UpdateShaderInput(const char* uniformName, float val);
		void UpdateShaderInput(const char* uniformName, glm::vec2 val);
		void UpdateShaderInput(const char* uniformName, glm::vec3 val);
		void UpdateShaderInput(const char* uniformName, glm::vec4 val);

		template<typename T>
		void RemoveShaderInput(const char* uniformName);
		
		/*static Material CreateBasicMaterial(){
			Material mat(new Shader("data/engine/shaders/default/opengl/default"));

			mat.UpdateShaderInput("color", glm::vec4(1.0,1.0,1.0,1.0));
			mat.UpdateShaderInput("shininess", 20.0f);
			mat.UpdateShaderInput("fogStart", 10.0f);
			mat.UpdateShaderInput("fogEnd", 20.0f);
			mat.UpdateShaderInput("fogColor", glm::vec4(0.2,0.2,0.2,0.2));
			
			mat.UpdateShaderInput("albedoMap", (Texture*)nullptr);
			mat.UpdateShaderInput("specularMap", glm::vec4(0.2,0.2,0.2,0.2));
			mat.UpdateShaderInput("normalMap", glm::vec4(0.2,0.2,0.2,0.2));

			
			mat.UpdateShaderInput("isTransparent", false);
			mat.UpdateShaderInput("cullFaces", true);

			return mat;
		}*/

		static Material CreatePBRMaterial(){}
		static Material CreateSpriteMaterial(){}
};