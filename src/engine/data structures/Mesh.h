#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<stdlib.h>
#include<iostream>
#include<vector>
#include<assimp/BaseImporter.h>
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Vertex.h"

class Mesh {
private:
	GLfloat* vertices;
	GLuint* indices;
	int verticesSize;
	int indicesSize;
	VAO vao = VAO();
	//EBO ebo = EBO(indices, indicesSize);
	//VBO vbo = VBO(vertices, verticesSize);
public:
	std::vector<Vertex> vertices2;
	std::vector<GLuint> indices2;
	EBO ebo = EBO(indices2);
	VBO vbo = VBO(vertices2);
	// ========== Constructors ==========
	
	Mesh(GLfloat* vertices, GLuint* indices, int verticesSize, int indicesSize);
	Mesh(std::vector<Vertex> vertices2, std::vector<GLuint> indices2);

	// ========== Getters ==========
	
	GLfloat* getVertices();
	GLuint* getIndices();
	int getVerticesSize();
	int getIndicesSize();
	VAO getVao();
	EBO getEbo();
	VBO getVbo();

	// ========== Mesh Functions ==========
	
	void bind();
	void unbind();
	void free();
};

#endif