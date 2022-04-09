#pragma once

#include "../../utilities/dpch.h"

#include<glad/glad.h>

class EBO {
public:
	GLuint ID;
	EBO(GLuint* indices, GLsizeiptr size);
	EBO(std::vector<GLuint> indices);

	void Bind();
	void Unbind();
	void Delete();
};
