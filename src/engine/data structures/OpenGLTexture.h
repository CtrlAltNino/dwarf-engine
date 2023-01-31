#pragma once
#include<glad/glad.h>
#include<filesystem>

#include "ITexture.h"

class OpenGLTexture : public ITexture {
    private:
		/// @brief The OpenGL texture handle.
		GLuint ID;
	public:
        OpenGLTexture(std::filesystem::path path);
        GLuint GetTextureId();
};