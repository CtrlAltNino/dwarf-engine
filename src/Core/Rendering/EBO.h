#pragma once

#include <glad/glad.h>

namespace Dwarf {
	
	/// @brief Class that manages an Element Buffer Object, containing of indices.
	class EBO {
	public:
		/// @brief The OpenGL buffer ID for the element array buffer.
		GLuint ID;
		EBO(GLuint* indices, GLsizeiptr size);
		EBO(std::vector<GLuint> indices);

		/// @brief Binds the EBO.
		void Bind();

		/// @brief Unbinds the EBO.
		void Unbind();

		/// @brief Deletes the OpenGL buffer.
		void Delete();
	};
}