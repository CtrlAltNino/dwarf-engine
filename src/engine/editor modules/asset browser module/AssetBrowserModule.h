#pragma once

#include "../../../utilities/dpch.h"

#include<imgui_internal.h>

#include "../IModule.h"

class AssetBrowserModule : public IModule{
    private:
        std::filesystem::path assetDirectoryPath;
        std::filesystem::path currentDirectory;
        //IWindowManager* windowManager;
    public:
        AssetBrowserModule(IViewListener *listener, std::string path, int id);
        void RenderModuleWindow() override;
        void StartFrame() override {}
};