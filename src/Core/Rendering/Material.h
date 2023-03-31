#pragma once

#include <glm/glm.hpp>
#include <map>

#include "Core/Base.h"
#include "Core/Rendering/Shader.h"
#include "Core/Rendering/Texture.h"
//#include "Core/Asset/AssetReference.h"
#include "Core/UID.h"

namespace Dwarf {

	/// @brief Class representing a modular Material.
	class Material {
		public:
			Material(std::string name);
			Material(std::string name, Ref<Shader> shader);
			~Material();

			/// @brief Name of the material.
			std::string m_Name;

			/// @brief Shader program for this material.
			Ref<Shader> m_Shader;

			/// @brief Flag indicating if this is a transparent object.
			bool m_Transparent;

			/// @brief Textures used in this material.
			std::map<std::string, Ref<UID>> m_Textures;

			/// @brief Boolean uniforms.
			std::map<std::string, bool> m_BoolUniforms;

			/// @brief Integer uniforms.
			std::map<std::string, int> m_IntegerUniforms;

			/// @brief Float uniforms.
			std::map<std::string, float> m_FloatUniforms;

			/// @brief 2D vector uniforms.
			std::map<std::string, glm::vec2> m_Vector2Uniforms;
			
			/// @brief 3D vector uniforms.
			std::map<std::string, glm::vec3> m_Vector3Uniforms;
			
			/// @brief 4D vector uniforms.
			std::map<std::string, glm::vec4> m_Vector4Uniforms;

			std::string GetName();

			Ref<Shader> GetShader();

			void SetShader(Ref<Shader> shader);

			/// @brief Binds the material and its settings.
			//void Bind();

			/// @brief Updates the uniforms in the shader.
			//void UpdateUniforms();

			/// @brief Updates the matrices in the shader.
			/// @param modelMatrix Model matrix.
			/// @param viewMatrix View matrix.
			/// @param projectionMatrix Projection matrix.
			//void UpdateMVP(glm::mat4x4 modelMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 projectionMatrix);

			/// @brief Creates a new texture uniform.
			/// @param uniformName Name of the uniform.
			/// @param val Value of the uniform.
			void SetTexture(std::string uniformName, Ref<UID> val);
			
			/// @brief Creates a new boolean uniform.
			/// @param uniformName Name of the uniform.
			/// @param val Value of the uniform.
			void SetUniform(std::string uniformName, bool val);
			
			/// @brief Creates a new integer uniform.
			/// @param uniformName Name of the uniform.
			/// @param val Value of the uniform.
			void SetUniform(std::string uniformName, int val);
			
			/// @brief Creates a new float uniform.
			/// @param uniformName Name of the uniform.
			/// @param val Value of the uniform.
			void SetUniform(std::string uniformName, float val);
			
			/// @brief Creates a new 2D vector uniform.
			/// @param uniformName Name of the uniform.
			/// @param val Value of the uniform.
			void SetUniform(std::string uniformName, glm::vec2 val);
			
			/// @brief Creates a new 3D vector uniform.
			/// @param uniformName Name of the uniform.
			/// @param val Value of the uniform.
			void SetUniform(std::string uniformName, glm::vec3 val);
			
			/// @brief Creates a new 4D vector uniform.
			/// @param uniformName Name of the uniform.
			/// @param val Value of the uniform.
			void SetUniform(std::string uniformName, glm::vec4 val);

			void SetTransparency(bool transparent);

			bool IsTransparent();

			std::map<std::string, Ref<UID>> GetTextures();

			/// @brief Boolean uniforms.
			std::map<std::string, bool> GetBoolUniforms();

			/// @brief Integer uniforms.
			std::map<std::string, int> GetIntegerUniforms();

			/// @brief Float uniforms.
			std::map<std::string, float> GetFloatUniforms();

			/// @brief 2D vector uniforms.
			std::map<std::string, glm::vec2> Get2DUniforms();
			
			/// @brief 3D vector uniforms.
			std::map<std::string, glm::vec3> Get3DUniforms();
			
			/// @brief 4D vector uniforms.
			std::map<std::string, glm::vec4> Get4DUniforms();

			/// @brief Removes an uniform.
			/// @tparam T Type of the uniform to remove.
			/// @param uniformName Name of the uniform to remove.
			template<typename T>
			void RemoveShaderInput(std::string uniformName);
			
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

			// static Material CreatePBRMaterial(){}
			// static Material CreateSpriteMaterial(){}
	};
}