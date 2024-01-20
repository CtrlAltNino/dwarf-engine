#pragma once

#include <glm/glm.hpp>
#include <map>

#include "Core/Base.h"
#include "Core/Rendering/Shader.h"
#include "Core/Rendering/Texture.h"
#include "Core/Rendering/IShaderParameter.h"
#include "Core/UID.h"

namespace Dwarf
{

	/// @brief Class representing a modular Material.
	class Material
	{
	public:
		Material(std::string name);
		Material(std::string name, Ref<Shader> shader);
		~Material();

		static Ref<Material> s_DefaultMaterial;
		static Ref<Material> s_ErrorMaterial;
		static Ref<Material> s_GridMaterial;
		static void Init();

		/// @brief Name of the material.
		std::string m_Name;

		/// @brief Shader program for this material.
		Ref<Shader> m_Shader;

		/// @brief Flag indicating if this is a transparent object.
		bool m_Transparent;

		std::map<std::string, Ref<IShaderParameter>> m_Parameters;

		std::string GetName();

		Ref<Shader> GetShader();

		void SetShader(Ref<Shader> shader);

		template <typename T>
		void SetParameter(std::string identifier, T parameter);

		void SetParameter(std::string identifier, Ref<UID> value, ShaderParameterType type);

		void SetTransparency(bool transparent);

		bool IsTransparent();

		void GenerateUniforms();
	};
}