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
    switch (mGraphicsApi)
    {
      using enum GraphicsApi;
      case OpenGL:
        return std::make_unique<OpenGLShaderAssetSelector>(mLogger,
                                                           mAssetDatabase);
      case D3D12: break;
      case Metal: break;
      case Vulkan: break;
      case None: break;
    }

    return nullptr;
  }
}