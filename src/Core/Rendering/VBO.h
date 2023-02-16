#pragma once

#include <glad/glad.h>

#include "Core/Rendering/Vertex.h"

namespace Dwarf {

	/// @brief Class representing a array buffer.
	class VBO {
	public:
		/// @brief OpenGL handle of the buffer.
		GLuint ID;
		VBO(GLfloat* vertices, GLsizeiptr size);
		VBO(std::vector<Vertex> vertices);

		/// @brief Binds the buffer.
		void Bind();

		/// @brief Unbinds the buffer.
		void Unbind();

		/// @brief Deletes the buffer.
		void Delete();
	};
}