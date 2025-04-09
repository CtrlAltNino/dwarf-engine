#include "pch.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "TextureFactory.h"
#include "Utilities/ImageUtilities/TextureCommon.h"

namespace Dwarf
{
  TextureFactory::TextureFactory(GraphicsApi                       api,
                                 std::shared_ptr<IImageFileLoader> loader,
                                 std::shared_ptr<IDwarfLogger>     logger,
                                 std::shared_ptr<IVramTracker>     vramTracker)
    : mApi(api)
    , mImageFileLoader(std::move(loader))
    , mLogger(std::move(logger))
    , mVramTracker(std::move(vramTracker))
  {
    mLogger->LogDebug(Log("TextureFactory created.", "TextureFactory"));
  }

  TextureFactory::~TextureFactory()
  {
    mLogger->LogDebug(Log("TextureFactory destroyed.", "TextureFactory"));
  }

  auto
  TextureFactory::GetPixelCount(const TextureResolution& size,
                                const TextureType&       type) -> uint64_t
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

  auto
  TextureFactory::GetBytesPerPixel(const TextureFormat&   format,
                                   const TextureDataType& type) -> uint64_t
  {
    uint64_t channelCount = 0;
    uint64_t channelSize = 0;
    switch (format)
    {
      case TextureFormat::RED:
      case TextureFormat::STENCIL:
      case TextureFormat::DEPTH: channelCount = 1; break;
      case TextureFormat::RG:
      case TextureFormat::DEPTH_STENCIL: channelCount = 2; break;
      case TextureFormat::RGB: channelCount = 3; break;
      case TextureFormat::RGBA: channelCount = 4; break;
    }

    switch (type)
    {
      case TextureDataType::UNSIGNED_BYTE: channelSize = 8; break;
      case TextureDataType::UNSIGNED_SHORT: channelSize = 16; break;
      case TextureDataType::INT:
      case TextureDataType::UNSIGNED_INT:
      case TextureDataType::FLOAT: channelSize = 32; break;
    }

    return channelCount * channelSize;
  }

  auto
  TextureFactory::LoadTexture(
    const std::shared_ptr<TextureContainer>& textureData) const
    -> std::unique_ptr<ITexture>
  {
    mLogger->LogDebug(Log("Creating texture", "TextureFactory"));

    switch (mApi)
    {
      case GraphicsApi::None:
        mLogger->LogError(Log("Graphics API is not set.", "TextureFactory"));
        throw std::runtime_error("Graphics API is not set.");
        break;
      case GraphicsApi::OpenGL:
        {
          mLogger->LogInfo(Log("Created OpenGL texture", "TextureFactory"));
          return std::make_unique<OpenGLTexture>(
            textureData, mLogger, mVramTracker);
          break;
        }
      case GraphicsApi::Vulkan:
        mLogger->LogError(
          Log("Vulkan API has not been implemented yet.", "TextureFactory"));
        throw std::runtime_error("Vulkan API has not been implemented yet.");
        break;
      case GraphicsApi::D3D12:
#if _WIN32
        mLogger->LogError(
          Log("D3D12 API has not been implemented yet.", "TextureFactory"));
        throw std::runtime_error("D3D12 API has not been implemented yet.");
#elif __linux__
        mLogger->LogError(
          Log("D3D12 API has not been implemented yet.", "TextureFactory"));
        throw std::runtime_error("D3D12 API has not been implemented yet.");
#endif
        break;
    }

    mLogger->LogError(Log("Failed to create texture", "TextureFactory"));

    return nullptr;
  }

  auto
  TextureFactory::FromPath(std::filesystem::path texturePath) const
    -> std::unique_ptr<ITexture>
  {
    mLogger->LogDebug(Log("Creating texture from path: " + texturePath.string(),
                          "TextureFactory"));
    std::shared_ptr<TextureContainer> textureData =
      mImageFileLoader->LoadImageFile(texturePath);

    return LoadTexture(textureData);
  }

  auto
  TextureFactory::FromData(const std::shared_ptr<TextureContainer>& textureData)
    const -> std::unique_ptr<ITexture>
  {
    mLogger->LogDebug(Log("Creating texture from data", "TextureFactory"));
    return LoadTexture(textureData);
  }

  auto
  TextureFactory::Empty(const TextureType&       type,
                        const TextureFormat&     format,
                        const TextureDataType&   dataType,
                        const TextureResolution& size,
                        const TextureParameters& parameters,
                        int samples) const -> std::unique_ptr<ITexture>
  {
    mLogger->LogDebug(Log("Creating empty texture", "TextureFactory"));
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
          std::vector<uint32_t> data;
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

  auto
  TextureFactory::Empty(const TextureType&       type,
                        const TextureFormat&     format,
                        const TextureDataType&   dataType,
                        const TextureResolution& size,
                        uint32_t samples) const -> std::unique_ptr<ITexture>
  {
    mLogger->LogDebug(Log("Creating empty texture", "TextureFactory"));
    TextureParameters parameters;
    return Empty(type, format, dataType, size, parameters, samples);
  }

  auto
  TextureFactory::GetPlaceholderTexture() -> std::shared_ptr<ITexture>
  {
    if (!mPlaceholderTexture)
    {
      mPlaceholderTexture =
        std::move(FromPath("data/engine/img/textures/placeholder.png"));
    }

    return mPlaceholderTexture;
  }
} // namespace Dwarf