#pragma once

#include "../IModule.h"
#include<iostream>
#include<imgui_internal.h>
#include<filesystem>
#include<string>
//#include"../../windowing/IWindowManager.h"

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