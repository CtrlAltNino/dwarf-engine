#pragma once

#include "../../../utilities/dpch.h"

#include<imgui_internal.h>

#include "../IModule.h"

/// @brief GUI Module to display a window for the asset directory structure.
class AssetBrowserModule : public IModule{
    private:
        /// @brief Path to the asset directory of the currently opened project.
        std::filesystem::path assetDirectoryPath;

        /// @brief Path of the currently navigated directory.
        std::filesystem::path currentDirectory;
        //IWindowManager* windowManager;
    public:
        AssetBrowserModule(IViewListener *listener, std::string path, int id);

        /// @brief Renders the module window.
        void RenderModuleWindow() override;

        /// @brief Executes all pre frame tasks.
        void StartFrame() override {}
};