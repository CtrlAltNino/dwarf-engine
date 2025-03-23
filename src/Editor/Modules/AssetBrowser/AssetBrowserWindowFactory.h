#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Scene/IO/ISceneIO.h"
#include "Editor/Modules/AssetBrowser/AssetBrowserWindow.h"
#include "Editor/Modules/AssetBrowser/IAssetBrowserWindowFactory.h"
#include "Logging/IDwarfLogger.h"
#include <boost/di.hpp>

namespace Dwarf
{
  class AssetBrowserWindowFactory : public IAssetBrowserWindowFactory
  {
  private:
    AssetDirectoryPath                mAssetDirectoryPath;
    std::shared_ptr<IDwarfLogger>     mLogger;
    std::shared_ptr<ITextureFactory>  mTextureFactory;
    std::shared_ptr<IAssetDatabase>   mAssetDatabase;
    std::shared_ptr<IInputManager>    mInputManager;
    std::shared_ptr<IEditorSelection> mEditorSelection;
    std::shared_ptr<IMaterialIO>      mMaterialIO;
    std::shared_ptr<IMaterialFactory> mMaterialFactory;
    std::shared_ptr<IAssetMetadata>   mAssetMetadata;
    std::shared_ptr<IMaterialCreator> mMaterialCreator;
    std::shared_ptr<IFileHandler>     mFileHandler;
    std::shared_ptr<ISceneIO>         mSceneIO;

  public:
    AssetBrowserWindowFactory(const AssetDirectoryPath&     assetDirectoryPath,
                              std::shared_ptr<IDwarfLogger> logger,
                              std::shared_ptr<ITextureFactory>  textureFactory,
                              std::shared_ptr<IAssetDatabase>   assetDatabase,
                              std::shared_ptr<IInputManager>    inputManager,
                              std::shared_ptr<IEditorSelection> editorSelection,
                              std::shared_ptr<IMaterialIO>      materialIO,
                              std::shared_ptr<IMaterialFactory> materialFactory,
                              std::shared_ptr<IAssetMetadata>   assetMetadata,
                              std::shared_ptr<IMaterialCreator> materialCreator,
                              std::shared_ptr<IFileHandler>     fileHandler,
                              std::shared_ptr<ISceneIO>         sceneIO);

    ~AssetBrowserWindowFactory() override;

    /**
     * @brief Creates a default AssetBrowserWindow instance
     *
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] auto
    Create() const -> std::unique_ptr<AssetBrowserWindow> override;

    /**
     * @brief Creates a AssetBrowserWindow instance based off serialized data
     *
     * @param serializedModule Serialized data of a AssetBrowserWindow
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] auto
    Create(SerializedModule serializedModule) const
      -> std::unique_ptr<AssetBrowserWindow> override;
  };
} // namespace Dwarf