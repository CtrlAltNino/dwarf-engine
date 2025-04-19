#include "pch.hpp"

#include "AssetBrowserWindowFactory.hpp"
#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Editor/Modules/AssetBrowser/AssetBrowserWindow.hpp"

namespace Dwarf
{
  AssetBrowserWindowFactory::AssetBrowserWindowFactory(
    const AssetDirectoryPath&                     assetDirectoryPath,
    std::shared_ptr<IDwarfLogger>                 logger,
    std::shared_ptr<ITextureFactory>              textureFactory,
    std::shared_ptr<IAssetDatabase>               assetDatabase,
    std::shared_ptr<IInputManager>                inputManager,
    std::shared_ptr<IEditorSelection>             editorSelection,
    std::shared_ptr<IMaterialIO>                  materialIO,
    std::shared_ptr<IMaterialFactory>             materialFactory,
    std::shared_ptr<IAssetMetadata>               assetMetadata,
    std::shared_ptr<IMaterialCreator>             materialCreator,
    std::shared_ptr<IShaderCreator>               shaderCreator,
    std::shared_ptr<IFileHandler>                 fileHandler,
    std::shared_ptr<ISceneIO>                     sceneIO,
    std::shared_ptr<ILoadedScene>                 loadedScene,
    std::shared_ptr<IAssetBrowserListenerFactory> assetBrowserListenerFactory)
    : mAssetDirectoryPath(assetDirectoryPath)
    , mLogger(std::move(logger))
    , mTextureFactory(std::move(textureFactory))
    , mAssetDatabase(std::move(assetDatabase))
    , mInputManager(std::move(inputManager))
    , mEditorSelection(std::move(editorSelection))
    , mMaterialIO(std::move(materialIO))
    , mMaterialFactory(std::move(materialFactory))
    , mAssetMetadata(std::move(assetMetadata))
    , mMaterialCreator(std::move(materialCreator))
    , mShaderCreator(std::move(shaderCreator))
    , mFileHandler(std::move(fileHandler))
    , mSceneIO(std::move(sceneIO))
    , mLoadedScene(std::move(loadedScene))
    , mAssetBrowserListenerFactory(std::move(assetBrowserListenerFactory))
  {
    mLogger->LogDebug(
      Log("AssetBrowserWindowFactory created", "AssetBrowserWindowFactory"));
  }

  AssetBrowserWindowFactory::~AssetBrowserWindowFactory()
  {
    mLogger->LogDebug(
      Log("AssetBrowserWindowFactory destroyed", "AssetBrowserWindowFactory"));
  }

  auto
  AssetBrowserWindowFactory::Create() const
    -> std::unique_ptr<AssetBrowserWindow>
  {
    return std::make_unique<AssetBrowserWindow>(mAssetDirectoryPath,
                                                mLogger,
                                                mTextureFactory,
                                                mAssetDatabase,
                                                mInputManager,
                                                mEditorSelection,
                                                mMaterialIO,
                                                mMaterialFactory,
                                                mAssetMetadata,
                                                mMaterialCreator,
                                                mShaderCreator,
                                                mFileHandler,
                                                mSceneIO,
                                                mLoadedScene,
                                                mAssetBrowserListenerFactory);
  }

  auto
  AssetBrowserWindowFactory::Create(SerializedModule serializedModule) const
    -> std::unique_ptr<AssetBrowserWindow>
  {
    return std::make_unique<AssetBrowserWindow>(mAssetDirectoryPath,
                                                mLogger,
                                                mTextureFactory,
                                                mAssetDatabase,
                                                mInputManager,
                                                mEditorSelection,
                                                mMaterialIO,
                                                mMaterialFactory,
                                                mAssetMetadata,
                                                mMaterialCreator,
                                                mShaderCreator,
                                                mFileHandler,
                                                mSceneIO,
                                                mLoadedScene,
                                                mAssetBrowserListenerFactory,
                                                serializedModule);
  }
} // namespace Dwarf