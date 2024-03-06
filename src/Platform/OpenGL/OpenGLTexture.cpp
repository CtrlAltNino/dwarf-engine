#include "dpch.h"
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Utilities/FileHandler.h"
#include "Core/Asset/AssetMetaData.h"

namespace Dwarf
{

	// Constructor without meta data
	OpenGLTexture::OpenGLTexture(std::filesystem::path const &path)
	{
		int numColCh = 0;
		int width = 0;
		int height = 0;
		unsigned char *bytes = stbi_load(path.string().c_str(), &width, &height, &numColCh, 0);

		SetSize(glm::ivec2(width, height));

		glGenTextures(1, &m_Id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Id);

		if (FileHandler::CheckIfFileExists((path.string() + AssetMetaData::META_DATA_EXTENSION).c_str()))
		{
			// nlohmann::json metaData = AssetMetaData::GetMetaData(path);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			if (numColCh == 3)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GetSize().x, GetSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
			}
			else if (numColCh == 4)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GetSize().x, GetSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
			}
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			if (numColCh == 3)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GetSize().x, GetSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
			}
			else if (numColCh == 4)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GetSize().x, GetSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
			}
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(bytes);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		// Delete Texture
	}

	uintptr_t OpenGLTexture::GetTextureID()
	{
		return m_Id;
	}
}