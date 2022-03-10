#pragma once

#include "../IModule.h"
#include"../../rendering/IRenderTexture.h"
#include<iostream>
#include<imgui_internal.h>
#include<glm/vec2.hpp>

class SceneViewerModule : public IModule{
    private:
        //ImTextureID textureID;
        IRenderTexture* renderTexture;
        float aspectRatio;
        glm::vec2 resolution;
    public:
        SceneViewerModule(IViewListener *listener, IRenderTexture* renderTexture, int index);
        void RenderModuleWindow() override;
        ImTextureID GetTextureID();
};