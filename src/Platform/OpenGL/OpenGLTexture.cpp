#include "Core/Rendering/Texture.h"
#include "OpenGLTexture.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include "dpch.h"

#include "Core/Asset/AssetMetaData.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Utilities/FileHandler.h"
#include <iostream>

namespace Dwarf
{
  // A mapping function that maps TextureFormat to OpenGL format
  GLenum
  GetTextureFormat(TextureFormat format, TextureDataType dataType)
  {
    switch (format)
    {
      case TextureFormat::RED:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE: return GL_RED;
          case TextureDataType::UNSIGNED_SHORT: return GL_RED_INTEGER;
          case TextureDataType::INT: return GL_RED_INTEGER;
          case TextureDataType::UNSIGNED_INT: return GL_RED_INTEGER;
          case TextureDataType::FLOAT: return GL_RED;
        }
      case TextureFormat::RG:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE: return GL_RG;
          case TextureDataType::UNSIGNED_SHORT: return GL_RG_INTEGER;
          case TextureDataType::INT: return GL_RG_INTEGER;
          case TextureDataType::UNSIGNED_INT: return GL_RG_INTEGER;
          case TextureDataType::FLOAT: return GL_RG;
        }
      case TextureFormat::RGB:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE: return GL_RGB;
          case TextureDataType::UNSIGNED_SHORT: return GL_RGB_INTEGER;
          case TextureDataType::INT: return GL_RGB_INTEGER;
          case TextureDataType::UNSIGNED_INT: return GL_RGB_INTEGER;
          case TextureDataType::FLOAT: return GL_RGB;
        }
      case TextureFormat::RGBA:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE: return GL_RGBA;
          case TextureDataType::UNSIGNED_SHORT: return GL_RGBA_INTEGER;
          case TextureDataType::INT: return GL_RGBA_INTEGER;
          case TextureDataType::UNSIGNED_INT: return GL_RGBA_INTEGER;
          case TextureDataType::FLOAT: return GL_RGBA;
        }
      case TextureFormat::DEPTH:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE: return GL_DEPTH_COMPONENT;
          case TextureDataType::UNSIGNED_SHORT: return GL_DEPTH_COMPONENT;
          case TextureDataType::INT: return GL_DEPTH_COMPONENT;
          case TextureDataType::UNSIGNED_INT: return GL_DEPTH_COMPONENT;
          case TextureDataType::FLOAT: return GL_DEPTH_COMPONENT;
        }
      case TextureFormat::STENCIL:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE: return GL_STENCIL_INDEX;
          case TextureDataType::UNSIGNED_SHORT: return GL_STENCIL_INDEX;
          case TextureDataType::INT: return GL_STENCIL_INDEX;
          case TextureDataType::UNSIGNED_INT: return GL_STENCIL_INDEX;
          case TextureDataType::FLOAT: return GL_STENCIL_INDEX;
        }
      case TextureFormat::DEPTH_STENCIL:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE: return GL_DEPTH_STENCIL;
          case TextureDataType::UNSIGNED_SHORT: return GL_DEPTH_STENCIL;
          case TextureDataType::INT: return GL_DEPTH_STENCIL;
          case TextureDataType::UNSIGNED_INT: return GL_DEPTH_STENCIL;
          case TextureDataType::FLOAT: return GL_DEPTH_STENCIL;
        }
    }
  }

  // A mapping function that maps TextureWrap to OpenGL wrap
  GLenum
  GetTextureWrap(TextureWrap wrap)
  {
    switch (wrap)
    {
      case TextureWrap::REPEAT: return GL_REPEAT;
      case TextureWrap::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
      case TextureWrap::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
      case TextureWrap::CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
    }
  }

  GLenum
  GetTextureMinFilter(TextureMinFilter filter)
  {
    switch (filter)
    {
      case TextureMinFilter::NEAREST: return GL_NEAREST;
      case TextureMinFilter::LINEAR: return GL_LINEAR;
      case TextureMinFilter::NEAREST_MIPMAP_NEAREST:
        return GL_NEAREST_MIPMAP_NEAREST;
      case TextureMinFilter::LINEAR_MIPMAP_NEAREST:
        return GL_LINEAR_MIPMAP_NEAREST;
      case TextureMinFilter::NEAREST_MIPMAP_LINEAR:
        return GL_NEAREST_MIPMAP_LINEAR;
      case TextureMinFilter::LINEAR_MIPMAP_LINEAR:
        return GL_LINEAR_MIPMAP_LINEAR;
    }
  }

  // A mapping function that maps TextureMagFilter to OpenGL mag filter
  GLenum
  GetTextureMagFilter(TextureMagFilter filter)
  {
    switch (filter)
    {
      case TextureMagFilter::NEAREST: return GL_NEAREST;
      case TextureMagFilter::LINEAR: return GL_LINEAR;
    }
  }

  // A Map that maps TextureType to OpenGL type
  GLenum
  GetTextureType(TextureType type, int samples)
  {
    switch (type)
    {
      case TextureType::TEXTURE_1D: return GL_TEXTURE_1D;
      case TextureType::TEXTURE_2D:
        if (samples > 1)
        {
          return GL_TEXTURE_2D_MULTISAMPLE;
        }
        return GL_TEXTURE_2D;
      case TextureType::TEXTURE_3D: return GL_TEXTURE_3D;
      case TextureType::TEXTURE_CUBE_MAP: return GL_TEXTURE_CUBE_MAP;
    }
  }

  GLenum
  GetTextureDataType(TextureDataType type)
  {
    switch (type)
    {
      case TextureDataType::UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
      case TextureDataType::UNSIGNED_SHORT: return GL_UNSIGNED_SHORT;
      case TextureDataType::INT: return GL_INT;
      case TextureDataType::UNSIGNED_INT: return GL_UNSIGNED_INT;
      case TextureDataType::FLOAT: return GL_FLOAT;
    }
  }

  GLenum
  GetInternalFormat(TextureFormat format, TextureDataType dataType)
  {
    switch (format)
    {
      case TextureFormat::RED:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE: return GL_R8;
          case TextureDataType::UNSIGNED_SHORT: return GL_R16UI;
          case TextureDataType::INT: return GL_R32I;
          case TextureDataType::UNSIGNED_INT: return GL_R32UI;
          case TextureDataType::FLOAT: return GL_R32F;
        }
      case TextureFormat::RG:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE: return GL_RG8;
          case TextureDataType::UNSIGNED_SHORT: return GL_RG16UI;
          case TextureDataType::INT: return GL_RG32I;
          case TextureDataType::UNSIGNED_INT: return GL_RG32UI;
          case TextureDataType::FLOAT: return GL_RG32F;
        }
      case TextureFormat::RGB:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE: return GL_RGB8;
          case TextureDataType::UNSIGNED_SHORT: return GL_RGB16UI;
          case TextureDataType::INT: return GL_RGB32I;
          case TextureDataType::UNSIGNED_INT: return GL_RGB32UI;
          case TextureDataType::FLOAT: return GL_RGB32F;
        }
      case TextureFormat::RGBA:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE: return GL_RGBA8;
          case TextureDataType::UNSIGNED_SHORT: return GL_RGBA16UI;
          case TextureDataType::INT: return GL_RGBA32I;
          case TextureDataType::UNSIGNED_INT: return GL_RGBA32UI;
          case TextureDataType::FLOAT: return GL_RGBA32F;
        }
      case TextureFormat::DEPTH:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE: return GL_DEPTH_COMPONENT;
          case TextureDataType::UNSIGNED_SHORT: return GL_DEPTH_COMPONENT16;
          case TextureDataType::INT: return GL_DEPTH_COMPONENT24;
          case TextureDataType::UNSIGNED_INT: return GL_DEPTH_COMPONENT32;
          case TextureDataType::FLOAT: return GL_DEPTH_COMPONENT32F;
        }
      case TextureFormat::STENCIL:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE: return GL_STENCIL_INDEX;
          case TextureDataType::UNSIGNED_SHORT: return GL_STENCIL_INDEX;
          case TextureDataType::INT: return GL_STENCIL_INDEX;
          case TextureDataType::UNSIGNED_INT: return GL_STENCIL_INDEX;
          case TextureDataType::FLOAT: return GL_STENCIL_INDEX;
        }
      case TextureFormat::DEPTH_STENCIL:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE: return GL_DEPTH24_STENCIL8;
          case TextureDataType::UNSIGNED_SHORT: return GL_DEPTH24_STENCIL8;
          case TextureDataType::INT: return GL_DEPTH24_STENCIL8;
          case TextureDataType::UNSIGNED_INT: return GL_DEPTH24_STENCIL8;
          case TextureDataType::FLOAT: return GL_DEPTH24_STENCIL8;
        }
    }
  }

  // A map that maps
  // Constructor without meta data
  OpenGLTexture::OpenGLTexture(Ref<TextureContainer>  data,
                               Ref<TextureParameters> parameters)
  {
    GLuint textureType = GetTextureType(data->Type, data->Samples);
    GLuint textureDataType = GetTextureDataType(data->DataType);
    GLuint textureFormat = GetTextureFormat(data->Format, data->DataType);
    GLuint textureWrapS = GetTextureWrap(parameters->WrapS);
    GLuint textureWrapT = GetTextureWrap(parameters->WrapT);
    GLuint textureWrapR = GetTextureWrap(parameters->WrapR);
    GLuint textureMinFilter = GetTextureMinFilter(parameters->MinFilter);
    GLuint textureMagFilter = GetTextureMagFilter(parameters->MagFilter);
    GLuint internalFormat = GetInternalFormat(data->Format, data->DataType);

    SetSize(glm::ivec3(data->Width, data->Height, 0));
    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
      // Handle OpenGL error
      std::cerr << "OpenGL error code: " << error << std::endl;
      // Clean up and delete resources
      // glDeleteTextures(1, &multisampleTextureID);
    }
    glCreateTextures(textureType, 1, &m_Id);

    glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, textureMinFilter);
    glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, textureMagFilter);

    switch (data->Type)
    {
      case TextureType::TEXTURE_1D:
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, textureWrapS);
        glTextureStorage1D(m_Id, 1, internalFormat, data->Width);
        if (data->ImageData)
        {
          glTextureSubImage1D(m_Id,
                              0,
                              0,
                              data->Width,
                              textureFormat,
                              textureDataType,
                              data->ImageData);
        }
        break;
      case TextureType::TEXTURE_2D:
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, textureWrapS);
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, textureWrapT);

        if (data->Samples > 1)
        {
          glTextureStorage2DMultisample(m_Id,
                                        data->Samples,
                                        internalFormat,
                                        data->Width,
                                        data->Height,
                                        GL_FALSE);
        }
        else
        {
          glTextureStorage2D(
            m_Id, 1, internalFormat, data->Width, data->Height);
        }

        if (data->ImageData)
        {
          glTextureSubImage2D(m_Id,
                              0,
                              0,
                              0,
                              data->Width,
                              data->Height,
                              textureFormat,
                              textureDataType,
                              data->ImageData);
        }
        break;
      case TextureType::TEXTURE_3D:
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, textureWrapS);
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, textureWrapT);
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_R, textureWrapR);

        glTextureStorage3D(
          m_Id, 1, internalFormat, data->Width, data->Height, data->Depth);

        if (data->ImageData)
        {
          glTextureSubImage3D(m_Id,
                              0,
                              0,
                              0,
                              0,
                              data->Width,
                              data->Height,
                              data->Depth,
                              textureFormat,
                              textureDataType,
                              data->ImageData);
        }
        break;
      case TextureType::TEXTURE_CUBE_MAP:
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, textureWrapS);
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, textureWrapT);
        glTextureParameteri(m_Id, GL_TEXTURE_WRAP_R, textureWrapR);

        // TODO: Implement cube map texture
        break;
    }

    glGenerateTextureMipmap(m_Id);
  }

  OpenGLTexture::~OpenGLTexture()
  {
    glDeleteTextures(1, &m_Id);
  }

  uintptr_t
  OpenGLTexture::GetTextureID()
  {
    return m_Id;
  }
}