#pragma once

#include <string>
#include <glm/glm.hpp>
#include <map>

#include "Core/Base.h"
#include "Core/Asset/AssetReference.h"

namespace Dwarf
{
	struct UniformCollection
	{
		/// @brief Textures used in this material.
		std::map<std::string, Ref<UID>> Textures;

		/// @brief Boolean uniforms.
		std::map<std::string, bool> Booleans;

		/// @brief Integer uniforms.
		std::map<std::string, int> Integers;

		/// @brief Float uniforms.
		std::map<std::string, float> Floats;

		/// @brief 2D vector uniforms.
		std::map<std::string, glm::vec2> Floats2D;

		/// @brief 3D vector uniforms.
		std::map<std::string, glm::vec3> Floats3D;

		/// @brief 4D vector uniforms.
		std::map<std::string, glm::vec4> Floats4D;
	};

	class Shader
	{
	protected:
		bool m_SuccessfullyCompiled;

	public:
		Shader();
		~Shader();

		virtual void Compile() = 0;

		virtual bool IsCompiled()
		{
			return m_SuccessfullyCompiled;
		}

		virtual UniformCollection GetUniforms() = 0;

		static Ref<Shader> Create();

		static void Init();

		static Ref<Shader> s_DefaultShader;
		static Ref<Shader> s_ErrorShader;
		static Ref<Shader> s_GridShader;

		static std::filesystem::path GetDefaultShaderPath();
		static std::filesystem::path GetErrorShaderPath();
		static std::filesystem::path GetGridShaderPath();
	};
}