#include "dpch.h"
#include "Core/Rendering/VBO.h"

namespace Dwarf {

	VBO::VBO(GLfloat* vertices, GLsizeiptr size) {
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VBO::VBO(std::vector<Vertex> vertices) {
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	}

	void VBO::Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}

	void VBO::Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VBO::Delete() {
		glDeleteBuffers(1, &ID);
	}
}