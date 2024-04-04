#include "Core/Rendering/Texture.h"
#include "dpch.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Utilities/FileHandler.h"
#include "Core/Asset/AssetMetaData.h"

namespace Dwarf
{
	// A Map that maps TextureFormat to OpenGL format
	static const std::map<TextureFormat, GLenum> s_TextureFormatMap = {
		{TextureFormat::RED, GL_RED},
		{TextureFormat::RG, GL_RG},
		{TextureFormat::RGB, GL_RGB},
		{TextureFormat::RGBA, GL_RGBA}};

	// A Map that maps TextureWrap to OpenGL wrap
	static const std::map<TextureWrap, GLenum> s_TextureWrapMap = {
		{TextureWrap::REPEAT, GL_REPEAT},
		{TextureWrap::MIRRORED_REPEAT, GL_MIRRORED_REPEAT},
		{TextureWrap::CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE},
		{TextureWrap::CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER}};

	// A Map that maps TextureMinFilter to OpenGL min filter
	static const std::map<TextureMinFilter, GLenum> s_TextureMinFilterMap = {
		{TextureMinFilter::NEAREST, GL_NEAREST},
		{TextureMinFilter::LINEAR, GL_LINEAR},
		{TextureMinFilter::NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST},
		{TextureMinFilter::LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST},
		{TextureMinFilter::NEAREST_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_LINEAR},
		{TextureMinFilter::LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR}};

	// A Map that maps TextureMagFilter to OpenGL mag filter
	static const std::map<TextureMagFilter, GLenum> s_TextureMagFilterMap = {
		{TextureMagFilter::NEAREST, GL_NEAREST},
		{TextureMagFilter::LINEAR, GL_LINEAR}};

	// A Map that maps TextureType to OpenGL type
	static const std::map<TextureType, GLenum> s_TextureTypeMap = {
		{TextureType::TEXTURE_1D, GL_TEXTURE_1D},
		{TextureType::TEXTURE_2D, GL_TEXTURE_2D},
		{TextureType::TEXTURE_3D, GL_TEXTURE_3D},
		{TextureType::TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP}};

	// A Map that maps TextureDataType to OpenGL data type
	static const std::map<TextureDataType, GLenum> s_TextureDataTypeMap = {
		{TextureDataType::UNSIGNED_BYTE, GL_UNSIGNED_BYTE},
		{TextureDataType::FLOAT, GL_FLOAT},
		{TextureDataType::UNSIGNED_INT, GL_UNSIGNED_INT},
		{TextureDataType::INT, GL_INT}};

	// A map that maps
	// Constructor without meta data
	OpenGLTexture::OpenGLTexture(TextureParameters const &parameters, glm::ivec3 size, void *data)
	{
		GLuint textureFormat = s_TextureFormatMap.at(parameters.Format);
		GLuint textureType = s_TextureTypeMap.at(parameters.Type);
		GLuint textureDataType = s_TextureDataTypeMap.at(parameters.DataType);
		GLuint textureWrapS = s_TextureWrapMap.at(parameters.WrapS);
		GLuint textureWrapT = s_TextureWrapMap.at(parameters.WrapT);
		GLuint textureWrapR = s_TextureWrapMap.at(parameters.WrapR);
		GLuint textureMinFilter = s_TextureMinFilterMap.at(parameters.MinFilter);
		GLuint textureMagFilter = s_TextureMagFilterMap.at(parameters.MagFilter);

		SetSize(glm::ivec3(size.x, size.y, size.z));

		glCreateTextures(textureType, 1, &m_Id);

		switch (parameters.Type)
		{
			using enum TextureType;
		case TEXTURE_1D:
			glTextureStorage1D(m_Id, 1, textureFormat, GetSize().x);
			glTextureSubImage1D(m_Id, 0, 0, GetSize().x, textureFormat, textureDataType, data);
			break;
		case TEXTURE_2D:
			glTextureStorage2D(m_Id, 1, textureFormat, GetSize().x, GetSize().y);
			glTextureSubImage2D(m_Id, 0, 0, 0, GetSize().x, GetSize().y, textureFormat, textureDataType, data);
			break;
		case TEXTURE_3D:
			glTextureStorage3D(m_Id, 1, textureFormat, GetSize().x, GetSize().y, GetSize().z);
			glTextureSubImage3D(m_Id, 0, 0, 0, 0, GetSize().x, GetSize().y, GetSize().z, textureFormat, textureDataType, data);
			break;
		case TEXTURE_CUBE_MAP:
			glTextureStorage2D(m_Id, 1, textureFormat, GetSize().x, GetSize().y);
			glTextureSubImage3D(m_Id, 0, 0, 0, 0, GetSize().x, GetSize().y, 6, textureFormat, textureDataType, data);
			break;
		}

		if (parameters.MinFilter != TextureMinFilter::UNSET)
		{
			glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, textureMinFilter);
		}

		if (parameters.MagFilter != TextureMagFilter::UNSET)
		{
			glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, textureMagFilter);
		}

		if (parameters.WrapS != TextureWrap::UNSET)
		{
			glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, textureWrapS);
		}

		if (parameters.WrapT != TextureWrap::UNSET)
		{
			glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, textureWrapT);
		}

		if (parameters.WrapR != TextureWrap::UNSET)
		{
			glTextureParameteri(m_Id, GL_TEXTURE_WRAP_R, textureWrapR);
		}

		glGenerateTextureMipmap(m_Id);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		// Delete Texture
		glDeleteTextures(1, &m_Id);
	}

	uintptr_t OpenGLTexture::GetTextureID()
	{
		return m_Id;
	}
}