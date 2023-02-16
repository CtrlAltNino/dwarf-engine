#include <string>

#include "Core/Rendering/Shader.h"

namespace Dwarf {

	std::string get_file_contents(std::string filename) {
		std::ifstream in(filename, std::ios::binary);
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

	void Shader::AddVertexShader(std::string filePath) {
		this->vertexShaderSource = get_file_contents(filePath);
	}

	void Shader::AddFragmentShader(std::string filePath) {
		this->fragmentShaderSource = get_file_contents(filePath);
	}

	void Shader::AddGeometryShader(std::string filePath) {
		this->geometryShaderSource = get_file_contents(filePath);
	}

	void Shader::CreateShaderProgram() {
		if(this->vertexShaderSource.length() > 0 && this->fragmentShaderSource.length() > 0) {
			const char* vertexSource = vertexShaderSource.c_str();
			const char* fragmentSource = fragmentShaderSource.c_str();

			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexSource, NULL);
			glCompileShader(vertexShader);

			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
			glCompileShader(fragmentShader);

			ID = glCreateProgram();

			glAttachShader(ID, vertexShader);
			glAttachShader(ID, fragmentShader);

			GLuint geometryShader = -1;

			if(geometryShaderSource.length() > 0) {
				const char* geometrySource = geometryShaderSource.c_str();

				GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
				glShaderSource(geometryShader, 1, &geometrySource, NULL);
				glCompileShader(geometryShader);
				
				glAttachShader(ID, geometryShader);
			}

			glLinkProgram(ID);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			if(geometryShader != -1) {
				glDeleteShader(geometryShader);
			}
		}else {
			// TODO log missing shader error
		}
	}

	void Shader::Activate() {
		glUseProgram(ID);
	}

	void Shader::Delete() {
		glDeleteProgram(ID);
	}
}