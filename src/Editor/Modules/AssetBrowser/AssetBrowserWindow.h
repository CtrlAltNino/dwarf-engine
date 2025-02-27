#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Creation/Material/IMaterialCreator.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Core/Asset/Metadata/IAssetMetadata.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Editor/Selection/IEditorSelection.h"
#include "Editor/Modules/IGuiModule.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "Input/IInputManager.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include <boost/serialization/strong_typedef.hpp>

#define RENAME_BUFFER_SIZE (128)

namespace Dwarf
{
  /// @brief GUI Module to display a window for the asset directory structure.
  class AssetBrowserWindow : public IGuiModule
  {
  private:
    /// @brief Path to the asset directory of the currently opened project.
    AssetDirectoryPath                m_AssetDirectoryPath;
    std::shared_ptr<ITextureFactory>  m_TextureFactory;
    std::shared_ptr<IAssetDatabase>   m_AssetDatabase;
    std::shared_ptr<IInputManager>    m_InputManager;
    std::shared_ptr<IEditorSelection> m_EditorSelection;
    std::shared_ptr<IMaterialIO>      m_MaterialIO;
    std::shared_ptr<IMaterialFactory> m_MaterialFactory;
    std::shared_ptr<IAssetMetadata>   m_AssetMetadata;
    std::shared_ptr<IMaterialCreator> m_MaterialCreator;
    std::shared_ptr<IFileHandler>     m_FileHandler;

    /// @brief Path of the currently navigated directory.
    std::filesystem::path m_CurrentDirectory;

    std::filesystem::path m_CopyPathBuffer;

    std::filesystem::path m_RenamePathBuffer;

    char m_RenameBuffer[RENAME_BUFFER_SIZE] = "";

    bool m_OpenRename;

    void
    SetRenameBuffer(std::filesystem::path const& path);

    std::vector<std::filesystem::path> m_DirectoryHistory;

    std::filesystem::path m_SelectedAsset;
    int                   m_HistoryPos = 0;
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

    float m_IconScale = 1.0f;

    std::unique_ptr<ITexture> m_DirectoryIcon;

    std::unique_ptr<ITexture> m_FBXIcon;
    std::unique_ptr<ITexture> m_OBJIcon;

    std::unique_ptr<ITexture> m_JPGIcon;
    std::unique_ptr<ITexture> m_PNGIcon;

    std::unique_ptr<ITexture> m_VertexShaderIcon;
    std::unique_ptr<ITexture> m_TessellationControlShaderIcon;
    std::unique_ptr<ITexture> m_TessellationEvaluationShaderIcon;
    std::unique_ptr<ITexture> m_GeometryShaderIcon;
    std::unique_ptr<ITexture> m_FragmentShaderIcon;
    std::unique_ptr<ITexture> m_ComputeShaderIcon;
    std::unique_ptr<ITexture> m_HLSLShaderIcon;

    std::unique_ptr<ITexture> m_SceneIcon;

    std::unique_ptr<ITexture> m_MaterialIcon;

    std::unique_ptr<ITexture> m_UnknownFileIcon;

    bool firstFrame = true;

    ImGuiID dockID;

    ImGuiID footerID;

    void
    RenderDirectoryLevel(std::filesystem::path const& directory);

    void
    SetupDockspace(ImGuiID id);

    void
    ClearSelection();

    void
    RenderFooter();

    void
    RenderFolderStructure();

    void
    RenderFolderContent();

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
                       std::shared_ptr<IFileHandler>     fileHandler);

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
                       SerializedModule                  serializedModule);

    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    /// @brief Executes all pre frame tasks.
    void
    OnUpdate() override;

    nlohmann::json
    Serialize() override;

    void
    Deserialize(nlohmann::json moduleData);
  };
}