#pragma once

#include<glad/glad.h>

#include"VBO.h"

/// @brief Class representing a vertex array
class VAO {
public:
	/// @brief OpenGL for the buffer.
	GLuint ID;
	VAO();
	
	//void LinkVBO(VBO VBO, GLuint layout);
	
	/// @brief 
	/// @param VBO 
	/// @param layout 
	/// @param numComponents 
	/// @param type 
	/// @param stride 
	/// @param offset 
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();
};