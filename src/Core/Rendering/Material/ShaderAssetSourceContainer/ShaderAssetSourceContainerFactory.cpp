#include "pch.hpp"

#include "Core/Base.hpp"
#include "Platform/OpenGL/OpenGLShaderAssetSourceContainer.hpp"
#include "ShaderAssetSourceContainerFactory.hpp"

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
    mLogger->LogDebug(Log("Creating ShaderAssetSourceContainer",
                          "ShaderAssetSourceContainerFactory"));

    switch (mGraphicsApi)
    {
      using enum GraphicsApi;
      case None:
        mLogger->LogError(
          Log("Graphics API is not set", "ShaderAssetSourceContainerFactory"));
        throw std::runtime_error("Graphics API is not set");
      case OpenGL:
        return std::make_unique<OpenGLShaderAssetSourceContainer>(
          mAssetDatabase.get(),
          mShaderSourceCollectionFactory,
          std::move(shaderSources));
      case Vulkan:
        mLogger->LogError(Log("Vulkan API has not been implemented yet",
                              "ShaderAssetSourceContainerFactory"));
        throw std::runtime_error("Vulkan API has not been implemented yet");
      case D3D12:
#ifdef _WIN32
        mLogger->LogError(Log("Direct3D12 API has not been implemented yet",
                              "GraphicsContextFactory"));
        throw std::runtime_error("Direct3D12 API has not been implemented yet");
#elif __linux__
        mLogger->LogError(Log("Direct3D12 is only supported on Windows",
                              "GraphicsContextFactory"));
        throw std::runtime_error("Direct3D12 is only supported on Windows");
#endif
    }

    mLogger->LogError(Log("Failed to create ShaderAssetSourceContainer",
                          "ShaderAssetSourceContainerFactory"));

    return nullptr;
  }
}