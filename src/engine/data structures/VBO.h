#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>
#include<vector>
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

#endif

