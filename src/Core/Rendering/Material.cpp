#include "dpch.h"
#include "Core/Rendering/Material.h"
#include "Core/Rendering/IShaderParameter.h"
#include "Core/Rendering/Shader Parameters/BooleanShaderParameter.h"
#include "Core/Rendering/Shader Parameters/FloatShaderParameter.h"
#include "Core/Rendering/Shader Parameters/IntegerShaderParameter.h"
#include "Core/Rendering/Shader Parameters/Tex2DShaderParameter.h"
#include "Core/Rendering/Shader Parameters/Vec2ShaderParameter.h"
#include "Core/Rendering/Shader Parameters/Vec3ShaderParameter.h"
#include "Core/Rendering/Shader Parameters/Vec4ShaderParameter.h"

namespace Dwarf
{

	Ref<Material> Material::s_DefaultMaterial = nullptr;
	Ref<Material> Material::s_ErrorMaterial = nullptr;
	Ref<Material> Material::s_GridMaterial = nullptr;

	void Material::Init()
	{
		s_DefaultMaterial = CreateRef<Material>("Default Material", Shader::s_DefaultShader);
		s_ErrorMaterial = CreateRef<Material>("Error Material", Shader::s_ErrorShader);
		s_GridMaterial = CreateRef<Material>("grid material", Shader::s_GridShader);
		s_GridMaterial->SetTransparency(true);
	}

	Material::Material(std::string name) : m_Name(name)
	{
		m_Shader = Shader::s_DefaultShader;
	}

	Material::Material(std::string name, Ref<Shader> shader) : m_Name(name), m_Shader(shader) {}

	Material::~Material() {}

	std::string Material::GetName()
	{
		return m_Name;
	}

	void Material::SetShader(Ref<Shader> shader)
	{
		m_Shader = shader;
	}

	Ref<Shader> Material::GetShader()
	{
		return m_Shader;
	}

	void Material::SetTransparency(bool transparent)
	{
		m_Transparent = transparent;
	}

	bool Material::IsTransparent()
	{
		return m_Transparent;
	}

	void Material::GenerateUniforms()
	{
		// Get all shader inputs from abstract Shader function and put them in the maps
		m_Parameters = m_Shader->GetParameters();
	}

	template <>
	void Material::SetParameter<bool>(std::string identifier, bool value)
	{
		if (m_Parameters.contains(identifier) && ((*m_Parameters[identifier]).GetType() == BOOLEAN))
		{
			(*std::dynamic_pointer_cast<BooleanShaderParameter>(m_Parameters[identifier])).m_Value = value;
		}
		else
		{
			m_Parameters[identifier] = CreateRef<BooleanShaderParameter>(BooleanShaderParameter(value));
		}
	}

	template <>
	void Material::SetParameter<int>(std::string identifier, int value)
	{
		if (m_Parameters.contains(identifier) && ((*m_Parameters[identifier]).GetType() == INTEGER))
		{
			(*std::dynamic_pointer_cast<IntegerShaderParameter>(m_Parameters[identifier])).m_Value = value;
		}
		else
		{
			m_Parameters[identifier] = CreateRef<IntegerShaderParameter>(IntegerShaderParameter(value));
		}
	}

	template <>
	void Material::SetParameter<float>(std::string identifier, float value)
	{
		if (m_Parameters.contains(identifier) && ((*m_Parameters[identifier]).GetType() == FLOAT))
		{
			(*std::dynamic_pointer_cast<FloatShaderParameter>(m_Parameters[identifier])).m_Value = value;
		}
		else
		{
			m_Parameters[identifier] = CreateRef<FloatShaderParameter>(FloatShaderParameter(value));
		}
	}

	template <>
	void Material::SetParameter<glm::vec2>(std::string identifier, glm::vec2 value)
	{
		if (m_Parameters.contains(identifier) && ((*m_Parameters[identifier]).GetType() == VEC2))
		{
			(*std::dynamic_pointer_cast<Vec2ShaderParameter>(m_Parameters[identifier])).m_Value = value;
		}
		else
		{
			m_Parameters[identifier] = CreateRef<Vec2ShaderParameter>(Vec2ShaderParameter(value));
		}
	}

	template <>
	void Material::SetParameter<glm::vec3>(std::string identifier, glm::vec3 value)
	{
		if (m_Parameters.contains(identifier) && ((*m_Parameters[identifier]).GetType() == VEC3))
		{
			(*std::dynamic_pointer_cast<Vec3ShaderParameter>(m_Parameters[identifier])).m_Value = value;
		}
		else
		{
			m_Parameters[identifier] = CreateRef<Vec3ShaderParameter>(Vec3ShaderParameter(value));
		}
	}

	template <>
	void Material::SetParameter<glm::vec4>(std::string identifier, glm::vec4 value)
	{
		if (m_Parameters.contains(identifier) && ((*m_Parameters[identifier]).GetType() == VEC4))
		{
			(*std::dynamic_pointer_cast<Vec4ShaderParameter>(m_Parameters[identifier])).m_Value = value;
		}
		else
		{
			m_Parameters[identifier] = CreateRef<Vec4ShaderParameter>(Vec4ShaderParameter(value));
		}
	}

	void Material::SetParameter(std::string identifier, Ref<UID> value, ShaderParameterType type)
	{
		if (m_Parameters.contains(identifier) && ((*m_Parameters[identifier]).GetType() == type))
		{
			switch (type)
			{
			case TEX2D:
				(*std::dynamic_pointer_cast<Tex2DShaderParameter>(m_Parameters[identifier])).m_Value = value;
				break;
			}
		}
		else
		{
			switch (type)
			{
			case TEX2D:
				m_Parameters[identifier] = CreateRef<Tex2DShaderParameter>(Tex2DShaderParameter(value));
				break;
			}
		}
	}
}