#include "TextureFactory.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include <cstdint>
#include <stdexcept>

#if _WIN32
#include "Platform/OpenGL/OpenGLTexture.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLTexture.h"
#elif __APPLE__
// #include "Platform/Metal/MetalShader.h"
#endif

namespace Dwarf
{
  TextureFactory::TextureFactory(GraphicsApi                       api,
                                 std::shared_ptr<IImageFileLoader> loader,
                                 std::shared_ptr<IDwarfLogger>     logger,
                                 std::shared_ptr<IVramTracker>     vramTracker)
    : m_Api(api)
    , m_ImageFileLoader(loader)
    , m_Logger(logger)
    , m_VramTracker(vramTracker)
  {
    m_Logger->LogDebug(Log("TextureFactory created.", "TextureFactory"));
  }

  TextureFactory::~TextureFactory()
  {
    m_Logger->LogDebug(Log("TextureFactory destroyed.", "TextureFactory"));
  }

  uint64_t
  TextureFactory::GetPixelCount(const TextureResolution& size,
                                const TextureType&       type)
  {
    switch (type)
    {
      case TextureType::TEXTURE_1D: return std::get<glm::ivec1>(size).x;
      case TextureType::TEXTURE_2D:
        return std::get<glm::ivec2>(size).x * std::get<glm::ivec2>(size).y;
      case TextureType::TEXTURE_3D:
        return std::get<glm::ivec3>(size).x * std::get<glm::ivec3>(size).y *
               std::get<glm::ivec3>(size).z;
      case TextureType::TEXTURE_CUBE_MAP:
        return std::get<glm::ivec2>(size).x * 2 * 6;
    }
    return 0;
  }

  uint64_t
  TextureFactory::GetBytesPerPixel(const TextureFormat&   format,
                                   const TextureDataType& type)
  {
    uint64_t channelCount = 0;
    uint64_t channelSize = 0;
    switch (format)
    {
      case TextureFormat::RED: channelCount = 1; break;
      case TextureFormat::RG: channelCount = 2; break;
      case TextureFormat::RGB: channelCount = 3; break;
      case TextureFormat::RGBA: channelCount = 4; break;
      case TextureFormat::DEPTH: channelCount = 1; break;
      case TextureFormat::STENCIL: channelCount = 1; break;
      case TextureFormat::DEPTH_STENCIL: channelCount = 2; break;
    }

    switch (type)
    {
      case TextureDataType::UNSIGNED_BYTE: channelSize = 8; break;
      case TextureDataType::UNSIGNED_SHORT: channelSize = 16; break;
      case TextureDataType::INT: channelSize = 32; break;
      case TextureDataType::UNSIGNED_INT: channelSize = 32; break;
      case TextureDataType::FLOAT: channelSize = 32; break;
    }

    return channelCount * channelSize;
  }

  std::shared_ptr<TextureParameters>
  TextureFactory::GetParameters(std::filesystem::path const& path)
  {
    // TODO: Implement getting the actual parameters from the file/meta data
    std::shared_ptr<TextureParameters> parameters =
      std::make_shared<TextureParameters>();
    parameters->WrapR = TextureWrap::REPEAT;
    parameters->WrapS = TextureWrap::REPEAT;
    parameters->WrapT = TextureWrap::REPEAT;
    parameters->MinFilter = TextureMinFilter::LINEAR;
    parameters->MagFilter = TextureMagFilter::LINEAR;
    return parameters;
  }

  std::unique_ptr<ITexture>
  TextureFactory::LoadTexture(std::shared_ptr<TextureContainer> textureData)
  {
    m_Logger->LogDebug(Log("Creating texture", "TextureFactory"));
    switch (m_Api)
    {
      case GraphicsApi::None:
        std::runtime_error("Error: No API selected.");
        break;
#if _WIN32
      case GraphicsApi::D3D12:
        std::runtime_error("D3D12 API has not been implemented yet.");
        break;
      case GraphicsApi::OpenGL:
        {
          m_Logger->LogInfo(Log("Created OpenGL texture", "TextureFactory"));
          return std::make_unique<OpenGLTexture>(
            textureData, m_Logger, m_VramTracker);
          break;
        }
      case GraphicsApi::Metal:
        std::runtime_error("Metal API has not been implemented yet.");
        break;
      case GraphicsApi::Vulkan:
        std::runtime_error("Vulkan API has not been implemented yet.");
        break;
#elif __linux__
      case GraphicsApi::D3D12:
        std::runtime_error("D3D12 API has not been implemented yet.");
        break;
      case GraphicsApi::OpenGL:
        {
          m_Logger->LogDebug(Log("Created OpenGL texture", "TextureFactory"));
          return std::make_unique<OpenGLTexture>(
            textureData, m_Logger, m_VramTracker);
          break;
        }
      case GraphicsApi::Metal:
        std::runtime_error("Metal API has not been implemented yet.");
        break;
      case GraphicsApi::Vulkan:
        std::runtime_error("Vulkan API has not been implemented yet.");
        break;
#elif __APPLE__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::OpenGL: break;
      case GraphicsApi::Metal:
        // return std::make_shared<MetalTexture>();
        break;
      case GraphicsApi::Vulkan: break;
#endif
    }

    m_Logger->LogError(Log("Failed to create texture", "TextureFactory"));

    return nullptr;
  }

  std::unique_ptr<ITexture>
  TextureFactory::FromPath(std::filesystem::path texturePath)
  {
    m_Logger->LogDebug(Log(
      "Creating texture from path: " + texturePath.string(), "TextureFactory"));
    std::string                        ext = texturePath.extension().string();
    std::shared_ptr<TextureParameters> parameters = GetParameters(texturePath);
    std::shared_ptr<TextureContainer>  textureData =
      m_ImageFileLoader->LoadTexture(texturePath);

    textureData->Parameters.MinFilter = TextureMinFilter::LINEAR_MIPMAP_LINEAR;
    textureData->Parameters.MipMapped = true;

    return LoadTexture(textureData);
  }

  std::unique_ptr<ITexture>
  TextureFactory::FromData(const std::shared_ptr<TextureContainer>& textureData)
  {
    m_Logger->LogDebug(Log("Creating texture from data", "TextureFactory"));
    return LoadTexture(textureData);
  }

  std::unique_ptr<ITexture>
  TextureFactory::Empty(const TextureType&       type,
                        const TextureFormat&     format,
                        const TextureDataType&   dataType,
                        const TextureResolution& size,
                        const TextureParameters& parameters,
                        int                      samples)
  {
    m_Logger->LogDebug(Log("Creating empty texture", "TextureFactory"));
    std::shared_ptr<TextureContainer> textureData =
      std::make_shared<TextureContainer>();
    textureData->Size = size;
    textureData->Type = type;
    textureData->Format = format;
    textureData->DataType = dataType;
    textureData->Samples = samples;
    textureData->Parameters = parameters;
    // Calculate the size of the texture data
    size_t dataSize =
      GetPixelCount(size, type) * GetBytesPerPixel(format, dataType);

    // Reserve space for the texture data
    // textureData->ImageData->resize(dataSize);
    // textureData->ImageData =
    //  calloc(GetPixelCount(size, type), GetBytesPerPixel(format, dataType));

    switch (dataType)
    {
      case TextureDataType::UNSIGNED_BYTE:
        {
          std::vector<unsigned char> data;
          data.resize(dataSize);
          textureData->ImageData = data;
          break;
        }
      case TextureDataType::UNSIGNED_SHORT:
        {
          std::vector<unsigned short> data;
          data.resize(dataSize);
          textureData->ImageData = data;
          break;
        }
      case TextureDataType::INT:
        {
          std::vector<int> data;
          data.resize(dataSize);
          textureData->ImageData = data;
          break;
        }
      case TextureDataType::UNSIGNED_INT:
        {
          std::vector<unsigned int> data;
          data.resize(dataSize);
          textureData->ImageData = data;
          break;
        }
      case TextureDataType::FLOAT:
        {
          std::vector<float> data;
          data.resize(dataSize);
          textureData->ImageData = data;
          break;
        }
    }

    return LoadTexture(textureData);
  }

  std::unique_ptr<ITexture>
  TextureFactory::Empty(const TextureType&       type,
                        const TextureFormat&     format,
                        const TextureDataType&   dataType,
                        const TextureResolution& size,
                        int                      samples)
  {
    m_Logger->LogDebug(Log("Creating empty texture", "TextureFactory"));
    TextureParameters parameters;
    return Empty(type, format, dataType, size, parameters, samples);
  }

  std::shared_ptr<ITexture>
  TextureFactory::GetPlaceholderTexture()
  {
    if (!m_PlaceholderTexture)
    {
      m_PlaceholderTexture =
        FromPath("data/engine/img/textures/placeholder.png");
    }

    return m_PlaceholderTexture;
  }
} // namespace Dwarf