#pragma once

#include "../../utilities/dpch.h"

#include<glad/glad.h>
#include<glm/glm.hpp>
// Material abstrahieren
#include<assimp/cimport.h>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<assimp/material.h>
#include"../rendering/BasicMaterial.h"
#include"../data structures/Mesh.h"

class Renderer {
private:
	Material material;
	//Mesh mesh;
	std::vector<Mesh> meshes;
public:
	// ========== Constructors ==========
	
	//Renderer(BasicMaterial* material, Mesh* mesh);
	Renderer(Material* material, const char* modelFileName);

	// ========== Getters ==========

	Material* getMaterial();
	//Mesh getMesh();

	// ========== Setters ==========

	void setMaterial(Material material);
	//void setMesh(Mesh* mesh);

	// ========== Renderer Functions ==========
	
	static std::vector<Mesh> meshFromFile(const char* fileName);
	void render(glm::mat4x4 modelMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 projectionMatrix);
};