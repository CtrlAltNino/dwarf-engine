#ifndef MATERIAL_CLASS_H
#define MATERIAL_CLASS_H

#include<glad/glad.h>
#include"../data structures/Color.h"
#include"Shader.h"
#include"../data structures/Texture.h"
#include<iostream>

class Material {
public:
	Color* color;
	Material(char* shaderPath, Color* const color);
};

#endif