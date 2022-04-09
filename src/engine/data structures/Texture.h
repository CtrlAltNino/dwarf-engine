#pragma once

#include "../../utilities/dpch.h"

#include<glad/glad.h>
#include<stb_image.h>

class Texture {
public:
	GLuint ID;
	int width = 0, height = 0;
	Texture() = default;
	Texture(const char* filePath);
	Texture(const char* filePath, GLint filter, GLint wrap, GLint colorFormat, GLint dataFormat);

	void Bind();
	void Unbind();
	void Delete();
	int GetHeight();
	int GetWidth();
};