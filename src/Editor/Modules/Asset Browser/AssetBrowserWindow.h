#pragma once

#include <imgui_internal.h>

#include "Core/Base.h"
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
        std::filesystem::path m_AssetDirectoryPath;

        /// @brief Path of the currently navigated directory.
        std::filesystem::path m_CurrentDirectory;

        std::filesystem::path m_CopyPathBuffer;

        std::filesystem::path m_RenamePathBuffer;

        char m_RenameBuffer[RENAME_BUFFER_SIZE] = "";

        bool m_OpenRename;

        void SetRenameBuffer(std::filesystem::path path);

        std::vector<std::filesystem::path> m_DirectoryHistory;

        std::filesystem::path m_SelectedAsset;
        int m_HistoryPos = 0;
        void OpenPath(std::filesystem::directory_entry directoryEntry);

        void EnterDirectory(std::filesystem::path path);

        void GoBack();

        void GoForward();

        void HandleShortcuts();

        void LoadIcons();

        void RenderDirectory(std::filesystem::path path);

        float m_IconScale = 1.0f;

        Ref<Texture> m_DirectoryIcon;

        Ref<Texture> m_FBXIcon;
        Ref<Texture> m_OBJIcon;

        Ref<Texture> m_JPGIcon;
        Ref<Texture> m_PNGIcon;

        Ref<Texture> m_VertexShaderIcon;
        Ref<Texture> m_TessellationControlShaderIcon;
        Ref<Texture> m_TessellationEvaluationShaderIcon;
        Ref<Texture> m_GeometryShaderIcon;
        Ref<Texture> m_FragmentShaderIcon;
        Ref<Texture> m_ComputeShaderIcon;
        Ref<Texture> m_HLSLShaderIcon;

        Ref<Texture> m_SceneIcon;

        Ref<Texture> m_MaterialIcon;

        Ref<Texture> m_UnknownFileIcon;

        bool firstFrame = true;

        ImGuiID dockID;

        ImGuiID footerID;

        void RenderDirectoryLevel(std::filesystem::path directory);

        void SetupDockspace(ImGuiID id);

        void SelectAsset(std::filesystem::path path);

        void ClearSelection();

        void RenderFooter();

        void RenderFolderStructure();

        void RenderFolderContent();

    public:
        AssetBrowserWindow(Ref<EditorModel> model, int id);

        /// @brief Renders the module window.
        void OnImGuiRender() override;

        /// @brief Executes all pre frame tasks.
        virtual void OnUpdate(double deltaTime) override;

        virtual std::string Serialize() override;

        virtual void Deserialize(nlohmann::json moduleData) override;
    };
}