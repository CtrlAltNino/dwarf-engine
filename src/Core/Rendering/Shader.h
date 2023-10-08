#pragma once

#include <string>

#include "Core/Base.h"
#include "Core/Asset/AssetReference.h"

namespace Dwarf {
	class Shader {
	protected:
		bool m_SuccessfullyCompiled;
	public:
		Shader();
		~Shader();

		virtual void Compile()=0;

		bool IsCompiled(){
			return m_SuccessfullyCompiled;
		}

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