#pragma once

#include <string>
#include <glad/glad.h>

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
	public:
		Shader();
		~Shader();
		// ID of the shader program
		//GLuint ID = -1;

		//void AddGeometryShader(std::string filePath);

		virtual void Compile()=0;

		//void Activate();
		//void Delete();
		static Ref<Shader> Create();

		static void Init();

		static Ref<Shader> s_DefaultShader;
		static Ref<Shader> s_ErrorShader;
		static Ref<Shader> s_GridShader;
	};
}