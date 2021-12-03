#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include<glad/glad.h>
#include<glm/glm.hpp>
// Material abstrahieren
#include<assimp/cimport.h>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<assimp/material.h>
#include"../rendering/BasicMaterial.h"
#include"../data/Mesh.h"

class Renderer {
private:
	BasicMaterial material;
	//Mesh mesh;
	std::vector<Mesh> meshes;
public:
	// ========== Constructors ==========
	
	//Renderer(BasicMaterial* material, Mesh* mesh);
	Renderer(BasicMaterial* material, const char* modelFileName);

	// ========== Getters ==========

	BasicMaterial* getMaterial();
	//Mesh getMesh();

	// ========== Setters ==========

	void setMaterial(BasicMaterial* material);
	//void setMesh(Mesh* mesh);

	// ========== Renderer Functions ==========
	
	static std::vector<Mesh> meshFromFile(const char* fileName);
	void render(glm::mat4x4 modelMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 projectionMatrix);
};

#endif