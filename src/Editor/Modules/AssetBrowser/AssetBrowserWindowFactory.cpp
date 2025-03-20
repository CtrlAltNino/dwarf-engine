#include "AssetBrowserWindowFactory.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/Modules/AssetBrowser/AssetBrowserWindow.h"
#include <memory>

namespace Dwarf
{
  AssetBrowserWindowFactory::AssetBrowserWindowFactory(
    AssetDirectoryPath                assetDirectoryPath,
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
    , mTextureFactory(textureFactory)
    , mAssetDatabase(assetDatabase)
    , mInputManager(inputManager)
    , mEditorSelection(editorSelection)
    , mMaterialIO(materialIO)
    , mMaterialFactory(materialFactory)
    , mAssetMetadata(assetMetadata)
    , mMaterialCreator(materialCreator)
    , mFileHandler(fileHandler)
    , mSceneIO(sceneIO)
  {
  }

  std::unique_ptr<AssetBrowserWindow>
  AssetBrowserWindowFactory::Create() const
  {
    return std::make_unique<AssetBrowserWindow>(mAssetDirectoryPath,
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

  std::unique_ptr<AssetBrowserWindow>
  AssetBrowserWindowFactory::Create(SerializedModule serializedModule) const
  {
    return std::make_unique<AssetBrowserWindow>(mAssetDirectoryPath,
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