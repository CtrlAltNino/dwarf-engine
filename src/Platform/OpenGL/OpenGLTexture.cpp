#include "OpenGLTexture.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "Platform/OpenGL/OpenGLUtilities.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include <SDL3/SDL_opengl.h>
#include <glm/fwd.hpp>

namespace Dwarf
{
  std::string
  GLenumToString(GLenum val)
  {
    switch (val)
    {
      case GL_TEXTURE_1D: return "GL_TEXTURE_1D";
      case GL_TEXTURE_2D: return "GL_TEXTURE_2D";
      case GL_TEXTURE_3D: return "GL_TEXTURE_3D";
      case GL_TEXTURE_CUBE_MAP: return "GL_TEXTURE_CUBE_MAP";
      case GL_TEXTURE_2D_MULTISAMPLE: return "GL_TEXTURE_2D_MULTISAMPLE";
      case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
        return "GL_TEXTURE_2D_MULTISAMPLE_ARRAY";
      case GL_TEXTURE_RECTANGLE: return "GL_TEXTURE_RECTANGLE";
      case GL_TEXTURE_BUFFER: return "GL_TEXTURE_BUFFER";
      case GL_TEXTURE_2D_ARRAY: return "GL_TEXTURE_2D_ARRAY";
      case GL_TEXTURE_CUBE_MAP_ARRAY: return "GL_TEXTURE_CUBE_MAP_ARRAY";
      case GL_TEXTURE_1D_ARRAY: return "GL_TEXTURE_1D_ARRAY";
      case GL_RED: return "GL_RED";
      case GL_RG: return "GL_RG";
      case GL_RGB: return "GL_RGB";
      case GL_RGBA: return "GL_RGBA";
      case GL_DEPTH_COMPONENT: return "GL_DEPTH_COMPONENT";
      case GL_STENCIL_INDEX: return "GL_STENCIL_INDEX";
      case GL_DEPTH_STENCIL: return "GL_DEPTH_STENCIL";
      case GL_R8: return "GL_R8";
      case GL_R16UI: return "GL_R16UI";
      case GL_R32I: return "GL_R32I";
      case GL_R32UI: return "GL_R32UI";
      case GL_R32F: return "GL_R32F";
      case GL_RG8: return "GL_RG8";
      case GL_RG16UI: return "GL_RG16UI";
      case GL_RG32I: return "GL_RG32I";
      case GL_RG32UI: return "GL_RG32UI";
      case GL_RG32F: return "GL_RG32F";
      case GL_RGB8: return "GL_RGB8";
      case GL_RGB16UI: return "GL_RGB16UI";
      case GL_RGB32I: return "GL_RGB32I";
      case GL_RGB32UI: return "GL_RGB32UI";
      case GL_RGB32F: return "GL_RGB32F";
      case GL_RGBA8: return "GL_RGBA8";
      case GL_RGBA16UI: return "GL_RGBA16UI";
      case GL_RGBA32I: return "GL_RGBA32I";
      case GL_RGBA32UI: return "GL_RGBA32UI";
      case GL_RGBA32F: return "GL_RGBA32F";
      case GL_DEPTH_COMPONENT16: return "GL_DEPTH_COMPONENT16";
      case GL_DEPTH_COMPONENT24: return "GL_DEPTH_COMPONENT24";
      case GL_DEPTH_COMPONENT32: return "GL_DEPTH_COMPONENT32";
      case GL_DEPTH_COMPONENT32F: return "GL_DEPTH_COMPONENT32F";
      case GL_DEPTH24_STENCIL8: return "GL_DEPTH24_STENCIL8";
      case GL_DEPTH32F_STENCIL8: return "GL_DEPTH32F_STENCIL8";
      case GL_UNSIGNED_BYTE: return "GL_UNSIGNED_BYTE";
      case GL_UNSIGNED_SHORT: return "GL_UNSIGNED_SHORT";
      case GL_INT: return "GL_INT";
      case GL_UNSIGNED_INT: return "GL_UNSIGNED_INT";
      case GL_FLOAT: return "GL_FLOAT";
      case GL_NEAREST: return "GL_NEAREST";
      case GL_LINEAR: return "GL_LINEAR";
      case GL_NEAREST_MIPMAP_NEAREST: return "GL_NEAREST_MIPMAP_NEAREST";
      case GL_LINEAR_MIPMAP_NEAREST: return "GL_LINEAR_MIPMAP_NEAREST";
      case GL_NEAREST_MIPMAP_LINEAR: return "GL_NEAREST_MIPMAP_LINEAR";
      case GL_LINEAR_MIPMAP_LINEAR: return "GL_LINEAR_MIPMAP_LINEAR";
      case GL_REPEAT: return "GL_REPEAT";
      case GL_MIRRORED_REPEAT: return "GL_MIRRORED_REPEAT";
      case GL_CLAMP_TO_EDGE: return "GL_CLAMP_TO_EDGE";
      case GL_CLAMP_TO_BORDER: return "GL_CLAMP_TO_BORDER";
    }

    return "UNKNOWN";
  }
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

    return GL_NONE;
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
  GetTextureMinFilter(TextureMinFilter filter, bool mipMapped)
  {
    switch (filter)
    {
      case TextureMinFilter::NEAREST: return GL_NEAREST;
      case TextureMinFilter::LINEAR: return GL_LINEAR;
      case TextureMinFilter::NEAREST_MIPMAP_NEAREST:
        return mipMapped ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
      case TextureMinFilter::LINEAR_MIPMAP_NEAREST:
        return mipMapped ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;
      case TextureMinFilter::NEAREST_MIPMAP_LINEAR:
        return mipMapped ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;
      case TextureMinFilter::LINEAR_MIPMAP_LINEAR:
        return mipMapped ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
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
  GetInternalFormat(TextureFormat format, TextureDataType dataType, bool srgb)
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
          case TextureDataType::UNSIGNED_BYTE: return srgb ? GL_SRGB8 : GL_RGB8;
          case TextureDataType::UNSIGNED_SHORT: return GL_RGB16UI;
          case TextureDataType::INT: return GL_RGB32I;
          case TextureDataType::UNSIGNED_INT: return GL_RGB32UI;
          case TextureDataType::FLOAT: return GL_RGB32F;
        }
      case TextureFormat::RGBA:
        switch (dataType)
        {
          case TextureDataType::UNSIGNED_BYTE:
            return srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8;
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

  const void*
  GetPixelPointer(const TextureContainer& data)
  {
    switch (data.DataType)
    {
      case TextureDataType::UNSIGNED_BYTE:
        return std::get<std::vector<unsigned char>>(data.ImageData).data();
      case TextureDataType::UNSIGNED_SHORT:
        return std::get<std::vector<unsigned short>>(data.ImageData).data();
      case TextureDataType::INT:
        return std::get<std::vector<int>>(data.ImageData).data();
      case TextureDataType::UNSIGNED_INT:
        return std::get<std::vector<unsigned int>>(data.ImageData).data();
      case TextureDataType::FLOAT:
        return std::get<std::vector<float>>(data.ImageData).data();
    }

    return nullptr;
  }

  // A map that maps
  // Constructor without meta data
  OpenGLTexture::OpenGLTexture(std::shared_ptr<TextureContainer> data,
                               std::shared_ptr<IDwarfLogger>     logger,
                               std::shared_ptr<IVramTracker>     vramTracker)
    : m_Logger(logger)
    , m_VramTracker(vramTracker)
  {
    GLuint textureType = GetTextureType(data->Type, data->Samples);
    GLuint textureDataType = GetTextureDataType(data->DataType);
    GLuint textureFormat = GetTextureFormat(data->Format, data->DataType);
    GLuint textureWrapS = GetTextureWrap(data->Parameters.WrapS);
    GLuint textureWrapT = GetTextureWrap(data->Parameters.WrapT);
    GLuint textureWrapR = GetTextureWrap(data->Parameters.WrapR);
    GLuint textureMinFilter = GetTextureMinFilter(data->Parameters.MinFilter,
                                                  data->Parameters.MipMapped);
    GLuint textureMagFilter = GetTextureMagFilter(data->Parameters.MagFilter);
    GLuint internalFormat =
      GetInternalFormat(data->Format, data->DataType, data->Parameters.IsSRGB);

    m_Logger->LogDebug(Log("Creating OpenGL texture", "OpenGLTexture"));
    m_Logger->LogDebug(
      Log("Texture type: " + GLenumToString(textureType), "OpenGLTexture"));
    m_Logger->LogDebug(
      Log("Texture data type: " + GLenumToString(textureDataType),
          "OpenGLTexture"));
    m_Logger->LogDebug(
      Log("Texture format: " + GLenumToString(textureFormat), "OpenGLTexture"));
    m_Logger->LogDebug(Log("Internal format: " + GLenumToString(internalFormat),
                           "OpenGLTexture"));

    OpenGLUtilities::CheckOpenGLError(
      "Before creating texture", "OpenGLTexture", m_Logger);
    glCreateTextures(textureType, 1, &m_Id);
    OpenGLUtilities::CheckOpenGLError(
      "glCreateTextures", "OpenGLTexture", m_Logger);

    glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, textureMinFilter);
    OpenGLUtilities::CheckOpenGLError(
      "glTextureParameteri MIN FILTER", "OpenGLTexture", m_Logger);

    glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, textureMagFilter);
    OpenGLUtilities::CheckOpenGLError(
      "glTextureParameteri MAG FILTER", "OpenGLTexture", m_Logger);

    if (GLAD_GL_EXT_texture_filter_anisotropic)
    {
      GLfloat maxAniso = 0.0f;
      glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
      OpenGLUtilities::CheckOpenGLError(
        "glGetFloatv GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT",
        "OpenGLTexture",
        m_Logger);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
      OpenGLUtilities::CheckOpenGLError(
        "glTexParameterf GL_TEXTURE_MAX_ANISOTROPY_EXT",
        "OpenGLTexture",
        m_Logger);
    }

    switch (data->Type)
    {
      case TextureType::TEXTURE_1D:
        {
          m_Logger->LogDebug(Log("Creating 1D texture", "OpenGLTexture"));
          glm::ivec1 size = std::get<glm::ivec1>(data->Size);
          glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, textureWrapS);
          OpenGLUtilities::CheckOpenGLError(
            "glTextureParameteri WRAP S", "OpenGLTexture", m_Logger);

          glTextureStorage1D(m_Id, 1, internalFormat, size.x);
          OpenGLUtilities::CheckOpenGLError(
            "glTextureStorage1D", "OpenGLTexture", m_Logger);
          glTextureSubImage1D(m_Id,
                              0,
                              0,
                              size.x,
                              textureFormat,
                              textureDataType,
                              GetPixelPointer(*data));
          OpenGLUtilities::CheckOpenGLError(
            "glTextureSubImage1D", "OpenGLTexture", m_Logger);
          break;
        }
      case TextureType::TEXTURE_2D:
        {
          m_Logger->LogDebug(Log("Creating 2D texture", "OpenGLTexture"));
          glm::ivec2 size = std::get<glm::ivec2>(data->Size);
          glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, textureWrapS);
          OpenGLUtilities::CheckOpenGLError(
            "glTextureParameteri WRAP S", "OpenGLTexture", m_Logger);
          glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, textureWrapT);
          OpenGLUtilities::CheckOpenGLError(
            "glTextureParameteri WRAP T", "OpenGLTexture", m_Logger);

          if (data->Samples > 1)
          {
            glTextureStorage2DMultisample(
              m_Id, data->Samples, internalFormat, size.x, size.y, GL_FALSE);
            OpenGLUtilities::CheckOpenGLError(
              "glTextureStorage2DMultisample", "OpenGLTexture", m_Logger);
          }
          else
          {
            int mipLevels =
              data->Parameters.MipMapped
                ? std::floor(std::log2(std::max(size.x, size.y))) + 1
                : 1;
            glTextureStorage2D(m_Id, mipLevels, internalFormat, size.x, size.y);
            OpenGLUtilities::CheckOpenGLError(
              "glTextureStorage2D", "OpenGLTexture", m_Logger);
          }

          glTextureSubImage2D(m_Id,
                              0,
                              0,
                              0,
                              size.x,
                              size.y,
                              textureFormat,
                              textureDataType,
                              GetPixelPointer(*data));
          OpenGLUtilities::CheckOpenGLError(
            "glTextureSubImage2D", "OpenGLTexture", m_Logger);
          break;
        }
      case TextureType::TEXTURE_3D:
        {
          m_Logger->LogDebug(Log("Creating 3D texture", "OpenGLTexture"));
          glm::ivec3 size = std::get<glm::ivec3>(data->Size);
          glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, textureWrapS);
          OpenGLUtilities::CheckOpenGLError(
            "glTextureParameteri GL_TEXTURE_WRAP_S", "OpenGLTexture", m_Logger);
          glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, textureWrapT);
          OpenGLUtilities::CheckOpenGLError(
            "glTextureParameteri GL_TEXTURE_WRAP_T", "OpenGLTexture", m_Logger);
          glTextureParameteri(m_Id, GL_TEXTURE_WRAP_R, textureWrapR);
          OpenGLUtilities::CheckOpenGLError(
            "glTextureParameteri GL_TEXTURE_WRAP_R", "OpenGLTexture", m_Logger);

          glTextureStorage3D(m_Id, 1, internalFormat, size.x, size.y, size.z);
          OpenGLUtilities::CheckOpenGLError(
            "glTextureStorage3D", "OpenGLTexture", m_Logger);

          glTextureSubImage3D(m_Id,
                              0,
                              0,
                              0,
                              0,
                              size.x,
                              size.y,
                              size.z,
                              textureFormat,
                              textureDataType,
                              GetPixelPointer(*data));
          OpenGLUtilities::CheckOpenGLError(
            "glTextureSubImage3D", "OpenGLTexture", m_Logger);
          break;
        }
      case TextureType::TEXTURE_CUBE_MAP:
        {
          m_Logger->LogDebug(Log("Creating cube map texture", "OpenGLTexture"));
          glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, textureWrapS);
          OpenGLUtilities::CheckOpenGLError(
            "glTextureParameteri GL_TEXTURE_WRAP_S", "OpenGLTexture", m_Logger);
          glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, textureWrapT);
          OpenGLUtilities::CheckOpenGLError(
            "glTextureParameteri GL_TEXTURE_WRAP_T", "OpenGLTexture", m_Logger);
          glTextureParameteri(m_Id, GL_TEXTURE_WRAP_R, textureWrapR);
          OpenGLUtilities::CheckOpenGLError(
            "glTextureParameteri GL_TEXTURE_WRAP_R", "OpenGLTexture", m_Logger);

          // TODO: Implement cube map texture
          break;
        }
    }

    if (data->Parameters.MipMapped)
    {
      glGenerateTextureMipmap(m_Id);
      OpenGLUtilities::CheckOpenGLError(
        "glGenerateTextureMipmap", "OpenGLTexture", m_Logger);
    }

    m_VramMemory = m_VramTracker->AddTextureMemory(data);

    m_Logger->LogDebug(Log("OpenGL texture created", "OpenGLTexture"));
    m_Logger->LogDebug(
      Log("Texture ID: " + std::to_string(m_Id), "OpenGLTexture"));
  }

  OpenGLTexture::~OpenGLTexture()
  {
    m_Logger->LogDebug(Log("Deleting OpenGL texture", "OpenGLTexture"));
    OpenGLUtilities::CheckOpenGLError(
      "Before deleting texture", "OpenGLTexture", m_Logger);
    glDeleteTextures(1, &m_Id);
    OpenGLUtilities::CheckOpenGLError(
      "glDeleteTextures", "OpenGLTexture", m_Logger);
    m_VramTracker->RemoveTextureMemory(m_VramMemory);
  }

  uintptr_t
  OpenGLTexture::GetTextureID() const
  {
    return m_Id;
  }

  TextureResolution
  OpenGLTexture::GetSize() const
  {
    return m_Size;
  }
}