#pragma once

#include <imgui_internal.h>

#include "Editor/Modules/IModule.h"

namespace Dwarf {

    /// @brief GUI Module to display a window for the asset directory structure.
    class AssetBrowserModule : public IModule{
        private:
            /// @brief Path to the asset directory of the currently opened project.
            std::filesystem::path assetDirectoryPath;

            /// @brief Path of the currently navigated directory.
            std::filesystem::path currentDirectory;
            //IWindowManager* windowManager;
        public:
            AssetBrowserModule(IViewListener *listener, std::filesystem::path path, int id);

            /// @brief Renders the module window.
            void RenderModuleWindow() override;

            /// @brief Executes all pre frame tasks.
            void StartFrame() override {}
    };
}