#include"Texture.h"

Texture::Texture(const char* filePath, GLint filter, GLint wrap, GLint colorFormat, GLint dataFormat) {
	int numColCh;
	unsigned char* bytes = stbi_load(filePath, &this->width, &this->height, &numColCh, 0);

	glGenTextures(1, &this->ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

	glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, this->width, this->height, 0, colorFormat, dataFormat, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(bytes);
}

void Texture::Delete() {
	glDeleteTextures(1, &ID);
}