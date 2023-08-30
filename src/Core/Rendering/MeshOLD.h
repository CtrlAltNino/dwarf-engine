#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/BaseImporter.h>

#include "Core/Rendering/VAO.h"
#include "Core/Rendering/VBO.h"
#include "Core/Rendering/EBO.h"
#include "Core/Rendering/Vertex.h"

namespace Dwarf {

	/// @brief Class representing a mesh.
	class Mesh {
	private:
		/*
		/// @brief The vertices of the mesh, stores a float pointer.
		GLfloat* vertices;

		/// @brief The indices of the mesh.
		GLuint* indices;*/

		/// @brief Number of individual vertices in this mesh.
		int verticesSize;

		/// @brief Number of indices for this mesh.
		int indicesSize;

		/// @brief Object storing the OpenGL vertex array for this mesh.
		VAO vao = VAO();
		//EBO ebo = EBO(indices, indicesSize);
		//VBO vbo = VBO(vertices, verticesSize);
	public:
		/// @brief Material index of this mesh to assign a material.
		int materialIndex = 0;

		/// @brief List of vertices this mesh consists of.
		std::vector<Vertex> vertices2;

		/// @brief List of indices for the triangles of this mesh.
		std::vector<GLuint> indices2;

		/// @brief Object storing the element array buffer.
		EBO ebo = EBO(indices2);

		/// @brief Object storing the vertex array buffer.
		VBO vbo = VBO(vertices2);
		// ========== Constructors ==========

		//Mesh(GLfloat* vertices, GLuint* indices, int verticesSize, int indicesSize);
		Mesh(std::vector<Vertex> vertices2, std::vector<GLuint> indices2);

		// ========== Getters ==========

		/// @brief Returns the vertices of a mesh.
		/// @return The vertices.
		GLfloat* getVertices();

		/// @brief Returns the indices of a mesh.
		/// @return The indices.
		GLuint* getIndices();

		/// @brief Returns the number of vertices a mesh consists of.
		/// @return Number of vertices.
		int getVerticesSize();

		/// @brief Returns the number of indices of a mesh.
		/// @return Number of indices.
		int getIndicesSize();

		/// @brief Returns the vertex array object.
		/// @return The VAO instance of a mesh.
		VAO getVao();

		/// @brief Returns the element array buffer object.
		/// @return The EBO instance of a mesh.
		EBO getEbo();

		/// @brief Returns the vertex buffer object.
		/// @return The VBO instance of a mesh.
		VBO getVbo();

		// ========== Mesh Functions ==========

		/// @brief Binds the mesh for rendering.
		void bind();

		/// @brief Unbinds the mesh.
		void unbind();

		/// @brief Frees memory and cleans up OpenGL buffer.
		void free();
	};
}