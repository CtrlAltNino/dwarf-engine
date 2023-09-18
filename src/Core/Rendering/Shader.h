#pragma once

#include <string>

// TODO: REDO SHADERS
#include "Core/Base.h"
#include "Core/Asset/AssetReference.h"

namespace Dwarf {

	//std::string get_file_contents(const char* filename);

	class Shader {
	protected:
		//std::string vertexShaderSource;
		//std::string fragmentShaderSource;
		//std::string geometryShaderSource;
		bool m_SuccessfullyCompiled;
	public:
		Shader();
		~Shader();
		// ID of the shader program
		//GLuint ID = -1;

		//void AddGeometryShader(std::string filePath);

		virtual void Compile()=0;

		bool IsCompiled(){
			return m_SuccessfullyCompiled;
		}

		//void Activate();
		//void Delete();
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