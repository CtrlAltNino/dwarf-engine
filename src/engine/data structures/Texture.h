#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb_image.h>
#include<iostream>

class Texture {
public:
	GLuint ID;
	int width = 0, height = 0;
	Texture();
	Texture(const char* filePath, GLint filter, GLint wrap, GLint colorFormat, GLint dataFormat);

	void Bind();
	void Unbind();
	void Delete();
	int GetHeight();
	int GetWidth();
};

#endif