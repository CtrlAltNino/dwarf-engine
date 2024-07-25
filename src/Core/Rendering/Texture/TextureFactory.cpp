#include "TextureFactory.h"

#if _WIN32
#include "Platform/OpenGL/OpenGLTexture.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLTexture.h"
#elif __APPLE__
// #include "Platform/Metal/MetalShader.h"
#endif

namespace Dwarf
{
  TextureFactory::TextureFactory(GraphicsApi                     api,
                                 std::shared_ptr<ITextureLoader> loader)
    : m_Api(api)
    , m_Loader(loader)
  {
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

  std::shared_ptr<ITexture>
  TextureFactory::CreateTexture(std::filesystem::path texturePath)
  {
    std::string                        ext = texturePath.extension().string();
    std::shared_ptr<TextureParameters> parameters = GetParameters(texturePath);
    std::shared_ptr<TextureContainer>  textureData = nullptr;

    textureData = m_Loader->LoadTexture(texturePath);

    return LoadTexture(textureData, parameters);
  }

  std::shared_ptr<ITexture>
  TextureFactory::LoadTexture(std::shared_ptr<TextureContainer>  textureData,
                              std::shared_ptr<TextureParameters> parameters)
  {
    switch (m_Api)
    {
#if _WIN32
      case GraphicsApi::D3D12:
        // return std::make_shared<D3D12Texture>();
        break;
      case GraphicsApi::OpenGL: return std::make_shared<OpenGLTexture>(); break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::Vulkan:
        // return std::make_shared<VulkanTexture>();
        break;
#elif __linux__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::OpenGL:
        return std::make_shared<OpenGLTexture>(textureData, parameters);
        break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::Vulkan:
        // return std::make_shared<VulkanTexture>();
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

    return nullptr;
  }

  // std::shared_ptr<Texture>
  // TextureCreator::FromData(TextureParameters const& parameters,
  //                          glm::ivec2               size,
  //                          void*                    data)
  // {
  //   std::shared_ptr<TextureContainer> textureData =
  //     std::make_shared<TextureContainer>();
  //   textureData->Width = size.x;
  //   textureData->Height = size.y;
  //   textureData->Format = TextureFormat::RGBA;
  //   textureData->Type = TextureType::TEXTURE_2D;
  //   textureData->DataType = TextureDataType::UNSIGNED_BYTE;
  //   textureData->ImageData = (unsigned char*)data;

  //   return Texture::Create(textureData,
  //                          std::make_shared<TextureParameters>(parameters));
  // }

  // std::shared_ptr<Texture>
  // TextureCreator::Empty(glm::ivec2 size)
  // {
  //   std::shared_ptr<TextureContainer> textureData =
  //     std::make_shared<TextureContainer>();
  //   textureData->Width = size.x;
  //   textureData->Height = size.y;
  //   textureData->Format = TextureFormat::RGBA;
  //   textureData->Type = TextureType::TEXTURE_2D;
  //   textureData->DataType = TextureDataType::UNSIGNED_BYTE;
  //   textureData->ImageData = nullptr;

  //   return Texture::Create(textureData);
  // }

  // std::shared_ptr<Texture>
  // TextureCreator::Empty(FramebufferTextureSpecification const& parameters,
  //                       glm::ivec2                             size,
  //                       int                                    samples)
  // {
  //   std::shared_ptr<TextureContainer> textureData =
  //     std::make_shared<TextureContainer>();
  //   textureData->Width = size.x;
  //   textureData->Height = size.y;
  //   textureData->Format = TextureFormat::RGBA;
  //   textureData->Type = TextureType::TEXTURE_2D;
  //   textureData->DataType = TextureDataType::UNSIGNED_BYTE;
  //   textureData->ImageData = nullptr;
  //   textureData->Samples = samples;

  //   switch (parameters.TextureFormat)
  //   {
  //     using enum FramebufferTextureFormat;
  //     case RGBA8: textureData->Format = TextureFormat::RGBA; break;
  //     case RED_INTEGER:
  //       textureData->Format = TextureFormat::RED;
  //       textureData->DataType = TextureDataType::UNSIGNED_INT;
  //       break;
  //     case DEPTH24STENCIL8:
  //       textureData->Format = TextureFormat::DEPTH_STENCIL;
  //       break;
  //     case DEPTH:
  //       textureData->Format = TextureFormat::DEPTH;
  //       textureData->DataType = TextureDataType::FLOAT;
  //       break;
  //     case STENCIL: textureData->Format = TextureFormat::STENCIL; break;
  //     case None: break;
  //   }

  //   return Texture::Create(textureData);
  // }
} // namespace Dwarf