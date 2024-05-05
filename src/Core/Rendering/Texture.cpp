#include "dpch.h"

// Image loading libraries

#include "Core/Rendering/Renderer.h"

#ifdef WIN32
// #include "Platform/Direct3D12/D3D12Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"
// #include "Platform/Vulkan/VulkanTexture.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLTexture.h"
// #include "Platform/Vulkan/VulkanTexture.h"
#elif __APPLE__
// #include "Platform/Metal/MetalTexture.h"
#endif

#include <nlohmann/json.hpp>

namespace Dwarf
{
  std::shared_ptr<Texture>
  Texture::Create(std::shared_ptr<TextureContainer>  data,
                  std::shared_ptr<TextureParameters> parameters)
  {
    switch (Renderer::GetAPI())
    {
      using enum GraphicsApi;
      case D3D12:
#ifdef WIN32
        // return std::make_shared<D3D12Texture>(D3D12Texture(parameters,
        // data));
#endif
        break;
      case Metal:
#ifdef __APPLE__
        // return std::make_shared<MetalTexture>(MetalTexture(parameters,
        // data));
#endif
        break;
      case OpenGL:
#if defined(__linux__) || defined(WIN32)
        return std::make_shared<OpenGLTexture>(data, parameters);
#endif
        break;
      case Vulkan:
#if defined(__linux__) || defined(WIN32)
        // return std::make_shared<VulkanTexture>(VulkanTexture(parameters,
        // data));
#endif
        break;
    }

    return nullptr;
  }
} // namespace Dwarf