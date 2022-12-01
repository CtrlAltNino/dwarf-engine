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
	std::string vertexShaderSource;
	std::string fragmentShaderSource;
	std::string geometryShaderSource;
public:
	// ID of the shader program
	GLuint ID = -1;

	void AddVertexShader(std::string filePath);
	void AddFragmentShader(std::string filePath);
	void AddGeometryShader(std::string filePath);

	void CreateShaderProgram();

	void Activate();
	void Delete();
};