#pragma once

#include "../../utilities/dpch.h"

#include<glad/glad.h>

std::string get_file_contents(const char* filename);
struct vertexShaderName {
	const char* value;
	constexpr operator const char* () const { return value; }
};

struct fragmentShaderName {
	const char* value;
	constexpr operator const char* () const { return value; }
};

class Shader {
private:
	void InitializeShader(const char* vertexFile, const char* fragmentFile);
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);
	Shader(vertexShaderName, fragmentShaderName);
	Shader(const char* shaderName);

	void Activate();
	void Delete();
};