#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#include "Core/Rendering/Texture.h"

namespace Dwarf {

	Texture::Texture(const char* filePath) : Texture(filePath, GL_LINEAR, GL_REPEAT, GL_RGB, GL_UNSIGNED_BYTE) {

	}

	Texture::Texture(const char* filePath, GLint filter, GLint wrap, GLint colorFormat, GLint dataFormat) {
		int numColCh;
		unsigned char* bytes = stbi_load(filePath, &this->size.x, &this->size.y, &numColCh, 0);

		glGenTextures(1, &this->ID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->ID);

		// TODO FIX
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

		glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, this->size.x, this->size.y, 0, colorFormat, dataFormat, bytes);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(bytes);
	}

	glm::ivec2 Texture::GetTextureSize(){
		return this->size;
	}

	void Texture::Delete() {
		glDeleteTextures(1, &ID);
	}
}