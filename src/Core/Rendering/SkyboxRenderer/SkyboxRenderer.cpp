#include "Core/Asset/Database/AssetComponents.hpp"
#include "Core/Rendering/SkyboxTypes.hpp"
#include "pch.hpp"

#include "SkyboxRenderer.hpp"

#include <utility>

#include <utility>

namespace Dwarf
{
  SkyboxRenderer::SkyboxRenderer(
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
    const std::shared_ptr<ICubeMapGeneratorFactory>& cubeMapGeneratorFactory)
    : mLogger(std::move(logger))
    , mRendererApi(rendererApiFactory->Create())
    , mAssetDatabase(std::move(assetDatabase))
    , mLoadedScene(std::move(loadedScene))
    , mShaderRegistry(std::move(shaderRegistry))
    , mShaderSourceCollectionFactory(std::move(shaderSourceCollectionFactory))
    , mTextureFactory(std::move(textureFactory))
    , mCubeMapGenerator(cubeMapGeneratorFactory->Create())
  {
    mLoadedScene->RegisterLoadedSceneObserver(this);
    std::shared_ptr<IMesh> cubeMesh = meshFactory->CreateSkyboxCube();
    mSkyboxCubeMesh = meshBufferFactory->Create(cubeMesh);
  }

  void
  SkyboxRenderer::SetCamera(ICamera& camera)
  {
    mCamera = camera;
  }

  void
  SkyboxRenderer::Render()
  {
    if (mCamera.has_value())
    {
      switch (mCachedSourceType)
      {
        using enum SkyboxSource;
        case Color:
          if (mSkyboxShader != nullptr)
          {
            mRendererApi->RenderSkyboxIndexed(
              mSkyboxCubeMesh.get(), *mSkyboxShader, mCamera->get());
          }
          break;
        case Material:
          if (mCachedMaterialAsset.has_value())
          {
            mRendererApi->RenderSkyboxIndexed(
              mSkyboxCubeMesh.get(),
              mCachedMaterialAsset->get().GetMaterial(),
              mCamera->get());
          }
          break;
        case HDRI:
          if (mCubeMap)
          {
            mSkyboxShader->SetParameter("u_Skybox", mCubeMap);
          }
          else
          {
            // Todo GET TEXTURE IMMEDIATELY
            if (mCachedTextureAsset)
            {
              if (mCachedTextureAsset->get().IsLoaded())
              {
                mCubeMap = mCubeMapGenerator->FromEquirectangular(
                  mCachedTextureAsset->get().GetTexture(), 4096);
              }
              else
              {
                mCachedTextureAsset->get().GetTexture();
              }
            }
          }
          mRendererApi->RenderSkyboxIndexed(
            mSkyboxCubeMesh.get(), *mSkyboxShader, mCamera->get());
          break;
      }
    }
  }

  void
  SkyboxRenderer::OnSkyboxSettingsChanged()
  {
    switch (mLoadedScene->GetScene()
              .GetProperties()
              .GetSettings()
              .GetSkyboxSettings()
              .GetType())
    {
      using enum SkyboxSource;
      case Color:
        // Load simple color material + shader
        if ((mSkyboxShader == nullptr) || (mCachedSourceType != Color))
        {
          if (mSkyboxShader)
          {
            mSkyboxShader->RemoveParameter("u_Skybox");
            mCubeMap.reset();
          }
          mSkyboxShader = mShaderRegistry->GetOrCreate(
            mShaderSourceCollectionFactory
              ->CreateColorSkyboxShaderSourceCollection());
        }

        mSkyboxShader->SetParameter("u_SkyColor",
                                    mLoadedScene->GetScene()
                                      .GetProperties()
                                      .GetSettings()
                                      .GetSkyboxSettings()
                                      .GetColor());

        break;
      case Material:
        // Load material from asset database
        if (mSkyboxShader)
        {
          mSkyboxShader->RemoveParameter("u_Skybox");
          mCubeMap.reset();
        }

        mCubeMap = nullptr;
        if (mCachedAssetId != mLoadedScene->GetScene()
                                .GetProperties()
                                .GetSettings()
                                .GetSkyboxSettings()
                                .GetSkyboxMaterial())
        {
          mCachedAssetId = mLoadedScene->GetScene()
                             .GetProperties()
                             .GetSettings()
                             .GetSkyboxSettings()
                             .GetSkyboxMaterial();

          if (mCachedAssetId.has_value())
          {
            mCachedMaterialAsset = dynamic_cast<MaterialAsset&>(
              mAssetDatabase->Retrieve(mCachedAssetId.value())->GetAsset());
          }
        }
        break;
      case HDRI:
        // Build cube map from hdri texture
        // Load material and shader for cubemap rendering
        if ((mSkyboxShader == nullptr) || (mCachedSourceType != HDRI))
        {
          mSkyboxShader = mShaderRegistry->GetOrCreate(
            mShaderSourceCollectionFactory
              ->CreateHdriSkyboxShaderSourceCollection());
        }

        if (mCachedAssetId != mLoadedScene->GetScene()
                                .GetProperties()
                                .GetSettings()
                                .GetSkyboxSettings()
                                .GetHdri())
        {
          mCachedAssetId = mLoadedScene->GetScene()
                             .GetProperties()
                             .GetSettings()
                             .GetSkyboxSettings()
                             .GetHdri();

          if (mCachedAssetId.has_value())
          {
            mCachedTextureAsset = dynamic_cast<TextureAsset&>(
              mAssetDatabase->Retrieve(mCachedAssetId.value())->GetAsset());
          }
        }
        break;
    }

    mCachedSourceType = mLoadedScene->GetScene()
                          .GetProperties()
                          .GetSettings()
                          .GetSkyboxSettings()
                          .GetType();
  }

  void
  SkyboxRenderer::OnSceneLoad()
  {
    if (mLoadedScene->HasLoadedScene())
    {
      mLoadedScene->GetScene()
        .GetProperties()
        .GetSettings()
        .RegisterSceneSettingsObserver(this);
      OnSkyboxSettingsChanged();
    }
  }

  void
  SkyboxRenderer::OnSceneUnload()
  {
    if (mLoadedScene->HasLoadedScene())
    {
      mLoadedScene->GetScene()
        .GetProperties()
        .GetSettings()
        .UnregisterSceneSettingsObserver(this);
    }
  }
}