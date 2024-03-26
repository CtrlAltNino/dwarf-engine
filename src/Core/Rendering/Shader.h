#pragma once

#include <string>
#include <glm/glm.hpp>
#include <map>

#include "Core/Base.h"
#include "Core/Asset/AssetReference.h"
#include "Core/Rendering/IShaderParameter.h"

namespace Dwarf
{
	class Shader
	{
	private:
		bool m_SuccessfullyCompiled;

	public:
		Shader();
		virtual ~Shader();

		virtual void Compile() = 0;

		bool IsCompiled() const
		{
			return m_SuccessfullyCompiled;
		}

		void SetIsCompiled(bool isCompiled)
		{
			m_SuccessfullyCompiled = isCompiled;
		}

		virtual std::map<std::string, Ref<IShaderParameter>, std::less<>> GetParameters() = 0;

		static Ref<Shader> Create();

		static void Init();

		static Ref<Shader> s_DefaultShader;
		static Ref<Shader> s_ErrorShader;
		static Ref<Shader> s_GridShader;
		static Ref<Shader> s_PreviewShader;
		static Ref<Shader> s_IdShader;

		static std::filesystem::path GetDefaultShaderPath();
		static std::filesystem::path GetErrorShaderPath();
		static std::filesystem::path GetGridShaderPath();
		static std::filesystem::path GetPreviewShaderPath();
		static std::filesystem::path GetIdShaderPath();

		static Ref<IShaderParameter> CreateShaderParameter(ShaderParameterType type);
	};
}