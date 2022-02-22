#include"Shader.h"

std::string get_file_contents(const char* filename) {
	std::ifstream in(std::string(filename), std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* shaderName) {
	std::string vertexFile = std::string(shaderName) + ".vert";
	std::string fragmentFile = std::string(shaderName) + ".frag";

	InitializeShader(vertexFile.c_str(), fragmentFile.c_str());
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	InitializeShader(vertexFile, fragmentFile);
}

Shader::Shader(vertexShaderName vName, fragmentShaderName fName) {
	InitializeShader(vName, fName);
}

void Shader::InitializeShader(const char* vertexFile, const char* fragmentFile) {
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	glLinkProgram(ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate() {
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}