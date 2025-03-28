#include "pch.h"

#include "Core/Base.h"
#include "Platform/OpenGL/OpenGLShaderAssetSourceContainer.h"
#include "ShaderAssetSourceContainerFactory.h"

namespace Dwarf
{
  ShaderAssetSourceContainerFactory::ShaderAssetSourceContainerFactory(
    GraphicsApi                                                 api,
    std::shared_ptr<IDwarfLogger>                               logger,
    boost::di::extension::lazy<std::shared_ptr<IAssetDatabase>> assetDatabase,
    std::shared_ptr<IShaderSourceCollectionFactory>
      shaderSourceCollectionFactory)
    : mGraphicsApi(api)
    , mLogger(std::move(logger))
    , mAssetDatabase(assetDatabase)
    , mShaderSourceCollectionFactory(std::move(shaderSourceCollectionFactory))
  {
    mLogger->LogDebug(Log("ShaderAssetSourceContainerFactory created",
                          "ShaderAssetSourceContainerFactory"));
  }

  ShaderAssetSourceContainerFactory::~ShaderAssetSourceContainerFactory()
  {
    mLogger->LogDebug(Log("ShaderAssetSourceContainerFactory destroyed",
                          "ShaderAssetSourceContainerFactory"));
  }

  auto
  ShaderAssetSourceContainerFactory::Create(
    std::unique_ptr<IShaderSourceCollection> shaderSources) const
    -> std::unique_ptr<IShaderAssetSourceContainer>
  {
    switch (mGraphicsApi)
    {
      using enum GraphicsApi;
      case OpenGL:
        return std::make_unique<OpenGLShaderAssetSourceContainer>(
          mAssetDatabase.get(),
          mShaderSourceCollectionFactory,
          std::move(shaderSources));
      case Vulkan:
      case Metal:
      case None:
      case D3D12: break;
    }

    return nullptr;
  }
}