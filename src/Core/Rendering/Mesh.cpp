#include "dpch.h"

#include"Core/Rendering/Mesh.h"

// ========== Constructors ==========

/*Mesh::Mesh(GLfloat *vertices, GLuint *indices, int indicesSize, int verticesSize)
	: vao(), vertices(vertices), indices(indices), indicesSize(indicesSize), verticesSize(verticesSize){
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	
	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();
}*/

namespace Dwarf {

	Mesh::Mesh(std::vector<Vertex> vertices2, std::vector<GLuint> indices2) : vertices2(vertices2), indices2(indices2) {
		this->verticesSize = 0;
		this->indicesSize = 0;

		vao = VAO();
		vao.Bind();
		ebo = EBO(indices2);
		vbo = VBO(vertices2);
		
		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex tangents
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		// vertex biTangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, BiTangent));
		// vertex texture coords
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));

		vao.Unbind();
		vbo.Unbind();
		ebo.Unbind();
	}

	// ========== Getters ==========

	/*GLfloat* Mesh::getVertices() {
		return vertices;
	}

	GLuint* Mesh::getIndices() {
		return indices;
	}*/

	int Mesh::getVerticesSize() {
		return verticesSize;
	}
	int Mesh::getIndicesSize() {
		return indicesSize;
	}

	VAO Mesh::getVao() {
		return vao;
	}

	EBO Mesh::getEbo() {
		return ebo;
	}

	VBO Mesh::getVbo() {
		return vbo;
	}

	// ========== Mesh Functions ==========

	void Mesh::bind() {
		vao.Bind();
	}

	void Mesh::unbind() {
		vao.Unbind();
	}

	void Mesh::free() {
		vao.Delete();
		vbo.Delete();
		ebo.Delete();
	}
}