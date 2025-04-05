#include "pch.h"

#include "Core/Base.h"
#include "Platform/OpenGL/OpenGLShaderAssetSelector.h"
#include "ShaderAssetSelectorFactory.h"

namespace Dwarf
{
  ShaderAssetSelectorFactory::ShaderAssetSelectorFactory(
    GraphicsApi                   graphicsApi,
    std::shared_ptr<IDwarfLogger> logger,
    std::shared_ptr<IShaderSourceCollectionFactory>
                                    shaderSourceCollectionFactory,
    std::shared_ptr<IAssetDatabase> assetDatabase)
    : mGraphicsApi(graphicsApi)
    , mLogger(std::move(logger))
    , mShaderSourceCollectionFactory(std::move(shaderSourceCollectionFactory))
    , mAssetDatabase(std::move(assetDatabase))
  {
    mLogger->LogDebug(
      Log("ShaderAssetSelectorFactory created", "ShaderAssetSelectorFactory"));
  }

  ShaderAssetSelectorFactory::~ShaderAssetSelectorFactory()
  {
    mLogger->LogDebug(Log("ShaderAssetSelectorFactory destroyed",
                          "ShaderAssetSelectorFactory"));
  }

  auto
  ShaderAssetSelectorFactory::Create() -> std::unique_ptr<IShaderAssetSelector>
  {
    mLogger->LogDebug(
      Log("Creating ShaderAssetSelector", "ShaderAssetSelectorFactory"));

    switch (mGraphicsApi)
    {
      using enum GraphicsApi;
      case None:
        mLogger->LogError(
          Log("Graphics API is not set", "ShaderAssetSelectorFactory"));
        throw std::runtime_error("Graphics API is not set");
      case OpenGL:
        return std::make_unique<OpenGLShaderAssetSelector>(mLogger,
                                                           mAssetDatabase);
      case Vulkan:
        mLogger->LogError(Log("Vulkan API has not been implemented yet",
                              "ShaderAssetSelectorFactory"));
        throw std::runtime_error("Vulkan API has not been implemented yet");
      case D3D12:
#ifdef _WIN32
        mLogger->LogError(Log("Direct3D12 API has not been implemented yet",
                              "ShaderAssetSelectorFactory"));
        throw std::runtime_error("Direct3D12 API has not been implemented yet");
#elif __linux__
        mLogger->LogError(Log("Direct3D12 is only supported on Windows",
                              "ShaderAssetSelectorFactory"));
        throw std::runtime_error("Direct3D12 is only supported on Windows");
#endif
    }

    mLogger->LogError(Log("Failed to create ShaderAssetSelector",
                          "ShaderAssetSelectorFactory"));

    return nullptr;
  }
}