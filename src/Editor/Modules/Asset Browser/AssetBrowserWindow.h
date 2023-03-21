#pragma once

#include <imgui_internal.h>

#include "Core/Base.h"
#include "Editor/EditorModel.h"
#include "Editor/Modules/GuiModule.h"
#include "Core/Rendering/ITexture.h"

namespace Dwarf {

    /// @brief GUI Module to display a window for the asset directory structure.
    class AssetBrowserWindow : public GuiModule{
        private:
            /// @brief Path to the asset directory of the currently opened project.
            std::filesystem::path m_AssetDirectoryPath;

            /// @brief Path of the currently navigated directory.
            std::filesystem::path m_CurrentDirectory;

            std::vector<std::filesystem::path> m_DirectoryHistory;

            std::filesystem::path m_SelectedAsset;
            int m_HistoryPos = 0;
            //IWindowManager* windowManager;
            void EnterDirectory(std::filesystem::path path);
            void GoBack();
            
            void GoForward();

            void LoadIcons();

            void RenderDirectory(std::filesystem::path path);

            float m_IconScale = 1.5f;

            Ref<ITexture> m_DirectoryIcon;
            
            Ref<ITexture> m_FBXIcon;
            Ref<ITexture> m_OBJIcon;
            
            Ref<ITexture> m_JPGIcon;
            Ref<ITexture> m_PNGIcon;

            Ref<ITexture> m_VertexShaderIcon;
            Ref<ITexture> m_TessellationControlShaderIcon;
            Ref<ITexture> m_TessellationEvaluationShaderIcon;
            Ref<ITexture> m_GeometryShaderIcon;
            Ref<ITexture> m_FragmentShaderIcon;
            Ref<ITexture> m_ComputeShaderIcon;
            Ref<ITexture> m_MetalShaderIcon;
            Ref<ITexture> m_HLSLShaderIcon;

            Ref<ITexture> m_SceneIcon;

            Ref<ITexture> m_MaterialIcon;

            Ref<ITexture> m_UnknownFileIcon;

            void SelectAsset(std::filesystem::path path);

            void ClearSelection();
        public:
            AssetBrowserWindow(Ref<EditorModel>model, int id);

            /// @brief Renders the module window.
            void OnImGuiRender() override;

            /// @brief Executes all pre frame tasks.
            void OnUpdate(double deltaTime) override;
    };
}