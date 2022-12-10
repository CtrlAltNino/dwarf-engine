#pragma once

#include "../../utilities/dpch.h"

#include<glad/glad.h>
#include<glm/glm.hpp>

#include"../data structures/Color.h"
#include"Shader.h"
#include"../data structures/Texture.h"
#include"Shader.h"

/// @brief Class representing a modular Material.
class Material {
	private:
		/// @brief Textures used in this material.
		std::map<std::string, Texture> textures = std::map<std::string, Texture>();

		/// @brief Boolean uniforms.
		std::map<std::string, bool> boolUniforms;

		/// @brief Integer uniforms.
		std::map<std::string, int> integerUniforms;

		/// @brief Float uniforms.
		std::map<std::string, float> floatUniforms;

		/// @brief 2D vector uniforms.
		std::map<std::string, glm::vec2> vector2Uniforms;
		
		/// @brief 3D vector uniforms.
		std::map<std::string, glm::vec3> vector3Uniforms;
		
		/// @brief 4D vector uniforms.
		std::map<std::string, glm::vec4> vector4Uniforms;
	public:
		/// @brief Name of the material.
		const char* name;

		/// @brief Shader program for this material.
		Shader shader;

		/// @brief Flag indicating if this is a transparent object.
		bool isTransparent = false;
		Material();
		Material(const char* name, Shader* shader);
		~Material();

		/// @brief Binds the material and its settings.
		void bind();

		/// @brief Updates the uniforms in the shader.
		void UpdateUniforms();

		/// @brief Updates the matrices in the shader.
		/// @param modelMatrix Model matrix.
		/// @param viewMatrix View matrix.
		/// @param projectionMatrix Projection matrix.
		void UpdateMVP(glm::mat4x4 modelMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 projectionMatrix);

		/// @brief Creates a new texture uniform.
		/// @param uniformName Name of the uniform.
		/// @param val Value of the uniform.
		void SetUniform(const char* uniformName, Texture* val);
		
		/// @brief Creates a new boolean uniform.
		/// @param uniformName Name of the uniform.
		/// @param val Value of the uniform.
		void SetUniform(const char* uniformName, bool val);
		
		/// @brief Creates a new integer uniform.
		/// @param uniformName Name of the uniform.
		/// @param val Value of the uniform.
		void SetUniform(const char* uniformName, int val);
		
		/// @brief Creates a new float uniform.
		/// @param uniformName Name of the uniform.
		/// @param val Value of the uniform.
		void SetUniform(const char* uniformName, float val);
		
		/// @brief Creates a new 2D vector uniform.
		/// @param uniformName Name of the uniform.
		/// @param val Value of the uniform.
		void SetUniform(const char* uniformName, glm::vec2 val);
		
		/// @brief Creates a new 3D vector uniform.
		/// @param uniformName Name of the uniform.
		/// @param val Value of the uniform.
		void SetUniform(const char* uniformName, glm::vec3 val);
		
		/// @brief Creates a new 4D vector uniform.
		/// @param uniformName Name of the uniform.
		/// @param val Value of the uniform.
		void SetUniform(const char* uniformName, glm::vec4 val);

		/// @brief Removes an uniform.
		/// @tparam T Type of the uniform to remove.
		/// @param uniformName Name of the uniform to remove.
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