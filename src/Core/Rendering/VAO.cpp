#include "dpch.h"
#include "Core/Rendering/VAO.h"

namespace Dwarf {

	VAO::VAO() {
		glGenVertexArrays(1, &ID);
	}

	/*void VAO::LinkVBO(VBO VBO, GLuint layout) {
		VBO.Bind();
		glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		// Enable the Vertex Attribute so that OpenGL knows to use it
		glEnableVertexAttribArray(layout);
		VBO.Unbind();
	}*/

	void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
		VBO.Bind();
		glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
		// Enable the Vertex Attribute so that OpenGL knows to use it
		glEnableVertexAttribArray(layout);
		VBO.Unbind();
	}

	void VAO::Bind() {
		glBindVertexArray(ID);
	}

	void VAO::Unbind() {
		glBindVertexArray(0);
	}

	void VAO::Delete() {
		glDeleteVertexArrays(1, &ID);
	}
}