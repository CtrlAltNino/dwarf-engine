#pragma once

#include "AssetBrowserListener/IAssetBrowserListenerFactory.h"
#include "Core/Asset/Creation/Material/IMaterialCreator.h"
#include "Core/Asset/Creation/Shader/IShaderCreator.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Metadata/IAssetMetadata.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "Core/Scene/IO/ISceneIO.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "Editor/Modules/IGuiModule.h"
#include "Editor/Selection/IEditorSelection.h"
#include "Input/IInputManager.h"
#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  struct DirectoryData
  {
    std::filesystem::path      Path;
    std::vector<DirectoryData> Subdirectories;
  };

  struct DirectoryStructureCache
  {
    DirectoryData RootDirectoryData;
    bool          Valid = false;
    std::mutex    ValidityMutex;
  };

  struct DirectoryEntryData
  {
    std::filesystem::path Path;
    ImTextureID           Thumbnail;
    bool                  IsDirectory;
  };

  struct DirectoryContentCache
  {
    std::vector<DirectoryEntryData> Entries;
    bool                            Valid = false;
    std::mutex                      ValidityMutex;
  };

  struct AssetBrowserData
  {
    std::filesystem::path              CurrentDirectory;
    std::filesystem::path              CopyPathBuffer;
    std::filesystem::path              RenamePathBuffer;
    std::string                        RenameBuffer;
    bool                               OpenRename = false;
    std::vector<std::filesystem::path> DirectoryHistory;
    int                                HistoryPos = 0;
    float                              IconScale = 1.0F;
    std::string                        SearchBuffer;
  };

  /// @brief GUI Module to display a window for the asset directory structure.
  class AssetBrowserWindow : public IGuiModule
  {
  private:
    /// @brief Path to the asset directory of the currently opened project.
    AssetDirectoryPath                     mAssetDirectoryPath;
    std::shared_ptr<IDwarfLogger>          mLogger;
    std::shared_ptr<ITextureFactory>       mTextureFactory;
    std::shared_ptr<IAssetDatabase>        mAssetDatabase;
    std::shared_ptr<IInputManager>         mInputManager;
    std::shared_ptr<IEditorSelection>      mEditorSelection;
    std::shared_ptr<IMaterialIO>           mMaterialIO;
    std::shared_ptr<IMaterialFactory>      mMaterialFactory;
    std::shared_ptr<IAssetMetadata>        mAssetMetadata;
    std::shared_ptr<IMaterialCreator>      mMaterialCreator;
    std::shared_ptr<IShaderCreator>        mShaderCreator;
    std::shared_ptr<IFileHandler>          mFileHandler;
    std::shared_ptr<ISceneIO>              mSceneIo;
    std::shared_ptr<ILoadedScene>          mLoadedScene;
    std::unique_ptr<IAssetBrowserListener> mAssetBrowserListener;
    bool                                   mDockspaceIsSetup = false;

    AssetBrowserData        mData;
    DirectoryStructureCache mDirectoryStructureCache;
    DirectoryContentCache   mDirectoryContentCache;

    /// @brief IDs.
    ImGuiID dockID = 0;
    ImGuiID footerID = 0;

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

    void
    RenderDirectoryStructureCacheRecursively(const DirectoryData& data);

    void
    SetupDockspace(ImGuiID imguiId);

    void
    ClearSelection();

    void
    RenderFooter();

    /**
     * @brief Renders the directory structure on the left
     *
     */
    void
    RenderDirectoryStructure();

    void
    RenderDirectoryContent();

    void
    RebuildDirectoryContentCache(const std::filesystem::path& directory);

    auto
    GenerateDirectoryDataRecursively(const std::filesystem::path& directory)
      -> DirectoryData;

    void
    UnfoldSelectedDirectory();

    void
    RebuildDirectoryStructureCache();

    void
    OpenPath(const std::filesystem::path& path);

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

    auto
    GetTextureIdForDirectoryEntry(
      const std::filesystem::directory_entry& directoryEntry) -> ImTextureID;

    /**
     * @brief Thread safely invalidates the directory structure cache
     *
     */
    void
    InvalidateDirectoryStructureCache();

    /**
     * @brief Thread safely invalidates the directory content cache
     *
     */
    void
    InvalidateDirectoryContentCache();

  public:
    AssetBrowserWindow(AssetDirectoryPath                assetDirectoryPath,
                       std::shared_ptr<IDwarfLogger>     logger,
                       std::shared_ptr<ITextureFactory>  textureFactory,
                       std::shared_ptr<IAssetDatabase>   assetDatabase,
                       std::shared_ptr<IInputManager>    inputManager,
                       std::shared_ptr<IEditorSelection> editorSelection,
                       std::shared_ptr<IMaterialIO>      materialIO,
                       std::shared_ptr<IMaterialFactory> materialFactory,
                       std::shared_ptr<IAssetMetadata>   assetMetadata,
                       std::shared_ptr<IMaterialCreator> materialCreator,
                       std::shared_ptr<IShaderCreator>   shaderCreator,
                       std::shared_ptr<IFileHandler>     fileHandler,
                       std::shared_ptr<ISceneIO>         sceneIO,
                       std::shared_ptr<ILoadedScene>     loadedScene,
                       std::shared_ptr<IAssetBrowserListenerFactory>
                         assetBrowserListenerFactory);

    AssetBrowserWindow(
      AssetDirectoryPath                            assetDirectoryPath,
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
      std::shared_ptr<IAssetBrowserListenerFactory> assetBrowserListenerFactory,
      SerializedModule                              serializedModule);

    ~AssetBrowserWindow() override;

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