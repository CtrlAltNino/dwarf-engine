#include "Core/Rendering/Texture.h"
#include "OpenGLTexture.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include "dpch.h"

#include "Core/Asset/AssetMetaData.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Utilities/FileHandler.h"

namespace Dwarf
{
  // A Map that maps TextureFormat to OpenGL format
  static const std::map<TextureFormat, GLenum> s_TextureFormatMap = {
    { TextureFormat::RED, GL_RED },
    { TextureFormat::RG, GL_RG },
    { TextureFormat::RGB, GL_RGB },
    { TextureFormat::RGBA, GL_RGBA }
  };

  // A Map that maps TextureWrap to OpenGL wrap
  static const std::map<TextureWrap, GLenum> s_TextureWrapMap = {
    { TextureWrap::REPEAT, GL_REPEAT },
    { TextureWrap::MIRRORED_REPEAT, GL_MIRRORED_REPEAT },
    { TextureWrap::CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE },
    { TextureWrap::CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER }
  };

  // A Map that maps TextureMinFilter to OpenGL min filter
  static const std::map<TextureMinFilter, GLenum> s_TextureMinFilterMap = {
    { TextureMinFilter::NEAREST, GL_NEAREST },
    { TextureMinFilter::LINEAR, GL_LINEAR },
    { TextureMinFilter::NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST },
    { TextureMinFilter::LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST },
    { TextureMinFilter::NEAREST_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_LINEAR },
    { TextureMinFilter::LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR }
  };

  // A Map that maps TextureMagFilter to OpenGL mag filter
  static const std::map<TextureMagFilter, GLenum> s_TextureMagFilterMap = {
    { TextureMagFilter::NEAREST, GL_NEAREST },
    { TextureMagFilter::LINEAR, GL_LINEAR }
  };

  // A Map that maps TextureType to OpenGL type
  static const std::map<TextureType, GLenum> s_TextureTypeMap = {
    { TextureType::TEXTURE_1D, GL_TEXTURE_1D },
    { TextureType::TEXTURE_2D, GL_TEXTURE_2D },
    { TextureType::TEXTURE_3D, GL_TEXTURE_3D },
    { TextureType::TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP }
  };

  // A Map that maps TextureDataType to OpenGL data type
  static const std::map<TextureDataType, GLenum> s_TextureDataTypeMap = {
    { TextureDataType::UNSIGNED_BYTE, GL_UNSIGNED_BYTE },
    { TextureDataType::FLOAT, GL_FLOAT },
    { TextureDataType::UNSIGNED_INT, GL_UNSIGNED_INT },
    { TextureDataType::INT, GL_INT }
  };

  GLenum
  GetInternalFormat(TextureFormat format, TextureDataType dataType)
  {
    switch (format)
    {
      case TextureFormat::RED:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE: return GL_R8;
          case TextureDataType::FLOAT: return GL_R32F;
          case TextureDataType::UNSIGNED_INT: return GL_R32UI;
          case TextureDataType::INT: return GL_R32I;
        }
      case TextureFormat::RG:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE: return GL_RG8;
          case TextureDataType::FLOAT: return GL_RG32F;
          case TextureDataType::UNSIGNED_INT: return GL_RG32UI;
          case TextureDataType::INT: return GL_RG32I;
        }
      case TextureFormat::RGB:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE: return GL_RGB8;
          case TextureDataType::FLOAT: return GL_RGB32F;
          case TextureDataType::UNSIGNED_INT: return GL_RGB32UI;
          case TextureDataType::INT: return GL_RGB32I;
        }
      case TextureFormat::RGBA:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE: return GL_RGBA8;
          case TextureDataType::FLOAT: return GL_RGBA32F;
          case TextureDataType::UNSIGNED_INT: return GL_RGBA32UI;
          case TextureDataType::INT: return GL_RGBA32I;
        }
    }
  }

  // A map that maps
  // Constructor without meta data
  OpenGLTexture::OpenGLTexture(Ref<TextureParameters> parameters,
                               Ref<TextureContainer>  data)
  {
    GLuint textureFormat = s_TextureFormatMap.at(data->Format);
    GLuint textureType = s_TextureTypeMap.at(data->Type);
    GLuint textureDataType = s_TextureDataTypeMap.at(data->DataType);
    GLuint textureWrapS = s_TextureWrapMap.at(parameters->WrapS);
    GLuint textureWrapT = s_TextureWrapMap.at(parameters->WrapT);
    GLuint textureWrapR = s_TextureWrapMap.at(parameters->WrapR);
    GLuint textureMinFilter = s_TextureMinFilterMap.at(parameters->MinFilter);
    GLuint textureMagFilter = s_TextureMagFilterMap.at(parameters->MagFilter);
    GLuint internalFormat = GetInternalFormat(data->Format, data->DataType);

    SetSize(glm::ivec3(data->Width, data->Height, 0));
    glCreateTextures(textureType, 1, &m_Id);

    if (parameters->MinFilter != TextureMinFilter::UNSET)
    {
      glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, textureMinFilter);
    }

    if (parameters->MagFilter != TextureMagFilter::UNSET)
    {
      glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, textureMagFilter);
    }

    if (parameters->WrapS != TextureWrap::UNSET)
    {
      glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, textureWrapS);
    }

    if (parameters->WrapT != TextureWrap::UNSET)
    {
      glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, textureWrapT);
    }

    if (parameters->WrapR != TextureWrap::UNSET)
    {
      glTextureParameteri(m_Id, GL_TEXTURE_WRAP_R, textureWrapR);
    }

    switch (textureType)
    {
      case GL_TEXTURE_1D:
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, textureWrapS);
        break;
      case GL_TEXTURE_2D:
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, textureWrapS);
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, textureWrapT);

        glTextureStorage2D(m_Id, 1, internalFormat, data->Width, data->Height);

        glTextureSubImage2D(m_Id,
                            0,
                            0,
                            0,
                            data->Width,
                            data->Height,
                            textureFormat,
                            GL_UNSIGNED_BYTE,
                            data->ImageData);
        break;
      case GL_TEXTURE_3D:
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, textureWrapS);
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, textureWrapT);
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_R, textureWrapR);
        break;
      case GL_TEXTURE_CUBE_MAP:
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, textureWrapS);
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, textureWrapT);
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_R, textureWrapR);
        break;
    }

    glGenerateTextureMipmap(m_Id);
  }

  OpenGLTexture::~OpenGLTexture()
  {
    // Delete Texture
    // glDeleteTextures(1, &m_Id);
  }

  uintptr_t
  OpenGLTexture::GetTextureID()
  {
    return m_Id;
  }
}