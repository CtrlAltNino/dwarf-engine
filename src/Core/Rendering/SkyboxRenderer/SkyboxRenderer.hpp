#pragma once

#include "Core/Asset/Database/AssetComponents.hpp"
#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.hpp"
#include "Core/Rendering/CubemapGenerator/ICubemapGenerator.hpp"
#include "Core/Rendering/CubemapGenerator/ICubemapGeneratorFactory.hpp"
#include "Core/Rendering/Material/IMaterial.hpp"
#include "Core/Rendering/Material/IMaterialFactory.hpp"
#include "Core/Rendering/Mesh/MeshFactory.hpp"
#include "Core/Rendering/MeshBuffer/IMeshBuffer.hpp"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.hpp"
#include "Core/Rendering/RendererApi/IRendererApi.hpp"
#include "Core/Rendering/RendererApi/IRendererApiFactory.hpp"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.hpp"
#include "Core/Rendering/SkyboxTypes.hpp"
#include "Core/Rendering/Texture/ITexture.hpp"
#include "Core/Rendering/Texture/ITextureFactory.hpp"
#include "Core/Scene/Settings/ISceneSettingsObserver.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "Editor/LoadedScene/ILoadedSceneObserver.h"
#include "ISkyboxRenderer.hpp"
#include "Logging/IDwarfLogger.hpp"
#include <cstdint>

namespace Dwarf
{
  class SkyboxRenderer
    : public ISkyboxRenderer
    , public ISceneSettingsObserver
    , public ILoadedSceneObserver
  {
  private:
    std::shared_ptr<IDwarfLogger>    mLogger;
    std::shared_ptr<IRendererApi>    mRendererApi;
    std::shared_ptr<IAssetDatabase>  mAssetDatabase;
    std::shared_ptr<ILoadedScene>    mLoadedScene;
    std::shared_ptr<IShaderRegistry> mShaderRegistry;
    std::shared_ptr<IShaderSourceCollectionFactory>
                                       mShaderSourceCollectionFactory;
    std::shared_ptr<ITextureFactory>   mTextureFactory;
    std::shared_ptr<ICubemapGenerator> mCubemapGenerator;

    std::optional<std::reference_wrapper<ICamera>> mCamera;
    std::shared_ptr<IMeshBuffer>                   mSkyboxCubeMesh;

    SkyboxSource             mCachedSourceType = SkyboxSource::Color;
    std::shared_ptr<IShader> mSkyboxShader = nullptr;
    std::optional<UUID>      mCachedAssetId = std::nullopt;
    std::optional<std::reference_wrapper<MaterialAsset>> mCachedMaterialAsset =
      std::nullopt;
    std::optional<std::reference_wrapper<TextureAsset>> mCachedTextureAsset =
      std::nullopt;
    std::shared_ptr<ITexture> mCubemap;
    CubemapResolutionEnum     mCachedResolution;

    std::mutex  mCubemapGeneratorMutex;
    std::thread mCubemapGeneratorThread;

    void
    ResetCubemap();

  public:
    SkyboxRenderer(
      std::shared_ptr<IDwarfLogger>    logger,
      std::shared_ptr<IAssetDatabase>  assetDatabase,
      std::shared_ptr<ILoadedScene>    loadedScene,
      std::shared_ptr<IShaderRegistry> shaderRegistry,
      std::shared_ptr<IShaderSourceCollectionFactory>
                                                  shaderSourceCollectionFactory,
      std::shared_ptr<ITextureFactory>            textureFactory,
      const std::shared_ptr<IRendererApiFactory>& rendererApiFactory,
      const std::shared_ptr<IMeshFactory>&        meshFactory,
      const std::shared_ptr<IMeshBufferFactory>&  meshBufferFactory,
      const std::shared_ptr<ICubemapGeneratorFactory>& cubeMapGeneratorFactory);
    ~SkyboxRenderer() override = default;

    void
    SetCamera(ICamera& camera) override;

    void
    Render() override;

    void
    OnSkyboxSettingsChanged() override;

    void
    OnAntiAliasingSettingsChanged() override
    {
    }

    void
    OnExposureSettingsChanged() override
    {
    }

    void
    OnTonemapChanged() override
    {
    }

    void
    OnBloomSettingsChanged() override
    {
    }

    void
    OnSceneLoad() override;

    void
    OnSceneUnload() override;
  };
}