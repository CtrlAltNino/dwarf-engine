#pragma once

#include "../../utilities/dpch.h"

#include<glad/glad.h>
#include<glm/glm.hpp>

#include"../data structures/Color.h"
#include"Shader.h"
#include"../data structures/Texture.h"
#include"Shader.h"

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
		const char* name;
		Shader shader;
		bool isTransparent = false;
		Material();
		Material(const char* name, Shader* shader);
		~Material();

		void bind();
		void UpdateUniforms();
		void UpdateMVP(glm::mat4x4 modelMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 projectionMatrix);

		void SetUniform(const char* uniformName, Texture* val);
		void SetUniform(const char* uniformName, bool val);
		void SetUniform(const char* uniformName, int val);
		void SetUniform(const char* uniformName, float val);
		void SetUniform(const char* uniformName, glm::vec2 val);
		void SetUniform(const char* uniformName, glm::vec3 val);
		void SetUniform(const char* uniformName, glm::vec4 val);

		template<typename T>
		void RemoveShaderInput(const char* uniformName);
		
		/*static Material CreateBasicMaterial(){
			Material mat(new Shader("data/engine/shaders/default/opengl/default"));

			mat.SetUniform("color", glm::vec4(1.0,1.0,1.0,1.0));
			mat.SetUniform("shininess", 20.0f);
			mat.SetUniform("fogStart", 10.0f);
			mat.SetUniform("fogEnd", 20.0f);
			mat.SetUniform("fogColor", glm::vec4(0.2,0.2,0.2,0.2));
			
			mat.SetUniform("albedoMap", (Texture*)nullptr);
			mat.SetUniform("specularMap", glm::vec4(0.2,0.2,0.2,0.2));
			mat.SetUniform("normalMap", glm::vec4(0.2,0.2,0.2,0.2));

			
			mat.SetUniform("isTransparent", false);
			mat.SetUniform("cullFaces", true);

			return mat;
		}*/

		static Material CreatePBRMaterial(){}
		static Material CreateSpriteMaterial(){}
};