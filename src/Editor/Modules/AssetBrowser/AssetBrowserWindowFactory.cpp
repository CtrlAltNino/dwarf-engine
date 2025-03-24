#include "pch.h"

#include "AssetBrowserWindowFactory.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/Modules/AssetBrowser/AssetBrowserWindow.h"

namespace Dwarf
{
  AssetBrowserWindowFactory::AssetBrowserWindowFactory(
    const AssetDirectoryPath&         assetDirectoryPath,
    std::shared_ptr<IDwarfLogger>     logger,
    std::shared_ptr<ITextureFactory>  textureFactory,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IEditorSelection> editorSelection,
    std::shared_ptr<IMaterialIO>      materialIO,
    std::shared_ptr<IMaterialFactory> materialFactory,
    std::shared_ptr<IAssetMetadata>   assetMetadata,
    std::shared_ptr<IMaterialCreator> materialCreator,
    std::shared_ptr<IFileHandler>     fileHandler,
    std::shared_ptr<ISceneIO>         sceneIO)
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
    , mFileHandler(std::move(fileHandler))
    , mSceneIO(std::move(sceneIO))
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
                                                mFileHandler,
                                                mSceneIO);
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
                                                mFileHandler,
                                                mSceneIO,
                                                serializedModule);
  }
} // namespace Dwarf