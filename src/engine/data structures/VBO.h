#pragma once

#include "../../utilities/dpch.h"

#include<glad/glad.h>

#include"Vertex.h"

class VBO {
public:
	GLuint ID;
	VBO(GLfloat* vertices, GLsizeiptr size);
	VBO(std::vector<Vertex> vertices);

	void Bind();
	void Unbind();
	void Delete();
};