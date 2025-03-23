#pragma once

#include "Core/Asset/Creation/Material/IMaterialCreator.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Metadata/IAssetMetadata.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "Core/Scene/IO/ISceneIO.h"
#include "Editor/Modules/IGuiModule.h"
#include "Editor/Selection/IEditorSelection.h"
#include "Input/IInputManager.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  /// @brief GUI Module to display a window for the asset directory structure.
  class AssetBrowserWindow : public IGuiModule
  {
  private:
    /// @brief Path to the asset directory of the currently opened project.
    AssetDirectoryPath                mAssetDirectoryPath;
    std::shared_ptr<ITextureFactory>  mTextureFactory;
    std::shared_ptr<IAssetDatabase>   mAssetDatabase;
    std::shared_ptr<IInputManager>    mInputManager;
    std::shared_ptr<IEditorSelection> mEditorSelection;
    std::shared_ptr<IMaterialIO>      mMaterialIO;
    std::shared_ptr<IMaterialFactory> mMaterialFactory;
    std::shared_ptr<IAssetMetadata>   mAssetMetadata;
    std::shared_ptr<IMaterialCreator> mMaterialCreator;
    std::shared_ptr<IFileHandler>     mFileHandler;
    std::shared_ptr<ISceneIO>         mSceneIo;

    /// @brief Path of the currently navigated directory.
    std::filesystem::path              mCurrentDirectory;
    std::filesystem::path              mCopyPathBuffer;
    std::filesystem::path              mRenamePathBuffer;
    std::string                        mRenameBuffer;
    bool                               mOpenRename = false;
    std::vector<std::filesystem::path> mDirectoryHistory;
    std::filesystem::path              mSelectedAsset;
    int                                mHistoryPos = 0;
    float                              mIconScale = 1.0F;

    std::unique_ptr<ITexture> mDirectoryIcon;
    std::unique_ptr<ITexture> mFBXIcon;
    std::unique_ptr<ITexture> mOBJIcon;
    std::unique_ptr<ITexture> mJPGIcon;
    std::unique_ptr<ITexture> mPNGIcon;
    std::unique_ptr<ITexture> mVertexShaderIcon;
    std::unique_ptr<ITexture> mTessellationControlShaderIcon;
    std::unique_ptr<ITexture> mTessellationEvaluationShaderIcon;
    std::unique_ptr<ITexture> mGeometryShaderIcon;
    std::unique_ptr<ITexture> mFragmentShaderIcon;
    std::unique_ptr<ITexture> mComputeShaderIcon;
    std::unique_ptr<ITexture> mHLSLShaderIcon;
    std::unique_ptr<ITexture> mSceneIcon;
    std::unique_ptr<ITexture> mMaterialIcon;
    std::unique_ptr<ITexture> mUnknownFileIcon;

    bool    firstFrame = true;
    ImGuiID dockID = 0;
    ImGuiID footerID = 0;

    void
    RenderDirectoryLevel(std::filesystem::path const& directory);

    void
    SetupDockspace(ImGuiID imguiId);

    void
    ClearSelection();

    void
    RenderFooter();

    void
    RenderFolderStructure();

    void
    RenderFolderContent();

    void
    OpenPath(std::filesystem::directory_entry const& directoryEntry);

    void
    EnterDirectory(std::filesystem::path const& path);

    void
    GoBack();

    void
    GoForward();

    void
    HandleShortcuts() const;

    void
    LoadIcons();

    void
    RenderDirectory(std::filesystem::path const& path);

    void
    SetRenameBuffer(std::filesystem::path const& path);

  public:
    AssetBrowserWindow(AssetDirectoryPath                assetDirectoryPath,
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

    AssetBrowserWindow(AssetDirectoryPath                assetDirectoryPath,
                       std::shared_ptr<ITextureFactory>  textureFactory,
                       std::shared_ptr<IAssetDatabase>   assetDatabase,
                       std::shared_ptr<IInputManager>    inputManager,
                       std::shared_ptr<IEditorSelection> editorSelection,
                       std::shared_ptr<IMaterialIO>      materialIO,
                       std::shared_ptr<IMaterialFactory> materialFactory,
                       std::shared_ptr<IAssetMetadata>   assetMetadata,
                       std::shared_ptr<IMaterialCreator> materialCreator,
                       std::shared_ptr<IFileHandler>     fileHandler,
                       std::shared_ptr<ISceneIO>         sceneIO,
                       SerializedModule                  serializedModule);

    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    /// @brief Executes all pre frame tasks.
    void
    OnUpdate() override;

    auto
    Serialize() -> nlohmann::json override;

    void
    Deserialize(nlohmann::json moduleData);
  };
}