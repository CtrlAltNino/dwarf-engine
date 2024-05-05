#pragma once

#include <imgui_internal.h>

#include "Core/Base.h"
#include "Core/Asset/AssetDatabase.h"
#include "Editor/EditorModel.h"
#include "Editor/Modules/GuiModule.h"
#include "Core/Rendering/Texture.h"

#define RENAME_BUFFER_SIZE (128)

namespace Dwarf
{

  /// @brief GUI Module to display a window for the asset directory structure.
  class AssetBrowserWindow : public GuiModule
  {
  private:
    /// @brief Path to the asset directory of the currently opened project.
    std::filesystem::path m_AssetDirectoryPath =
      AssetDatabase::GetAssetDirectoryPath();

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

    std::shared_ptr<Texture> m_DirectoryIcon;

    std::shared_ptr<Texture> m_FBXIcon;
    std::shared_ptr<Texture> m_OBJIcon;

    std::shared_ptr<Texture> m_JPGIcon;
    std::shared_ptr<Texture> m_PNGIcon;

    std::shared_ptr<Texture> m_VertexShaderIcon;
    std::shared_ptr<Texture> m_TessellationControlShaderIcon;
    std::shared_ptr<Texture> m_TessellationEvaluationShaderIcon;
    std::shared_ptr<Texture> m_GeometryShaderIcon;
    std::shared_ptr<Texture> m_FragmentShaderIcon;
    std::shared_ptr<Texture> m_ComputeShaderIcon;
    std::shared_ptr<Texture> m_HLSLShaderIcon;

    std::shared_ptr<Texture> m_SceneIcon;

    std::shared_ptr<Texture> m_MaterialIcon;

    std::shared_ptr<Texture> m_UnknownFileIcon;

    bool firstFrame = true;

    ImGuiID dockID;

    ImGuiID footerID;

    void
    RenderDirectoryLevel(std::filesystem::path const& directory);

    void
    SetupDockspace(ImGuiID id);

    void
    SelectAsset(std::filesystem::path const& path);

    void
    ClearSelection();

    void
    RenderFooter();

    void
    RenderFolderStructure();

    void
    RenderFolderContent();

  public:
    AssetBrowserWindow(std::shared_ptr<EditorModel> model, int id);

    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    /// @brief Executes all pre frame tasks.
    void
    OnUpdate(double deltaTime) override;

    std::string
    Serialize() override;

    void
    Deserialize(nlohmann::json moduleData) override;
  };
}