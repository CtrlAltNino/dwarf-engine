#pragma once

#include"../../editor/EditorProperties.h"
#include "../IModule.h"
#include"../../rendering/IRenderTexture.h"
#include"../../scene/Camera.h"
#include<iostream>
#include<imgui_internal.h>
#include<glm/vec2.hpp>
#include<glm/vec3.hpp>
#include"../../data structures/Quaternion.h"
#include"../../input/IInputManager.h"

class SceneViewerModule : public IModule{
    private:
        //ImTextureID textureID;
        IRenderTexture* renderTexture;
        IInputManager* inputManager;
        IWindowManager* windowManager;

        float aspectRatio;
        glm::vec2 resolution;
        Camera* camera = nullptr;
        bool cameraUpdating = false;
        void UpdateCamera();
        glm::vec2 lastMousePos = glm::vec2(-1);
        glm::vec2 deltaMousePos = glm::vec2(0);
    public:
        SceneViewerModule(IViewListener *listener, IRenderTexture* renderTexture, IInputManager* inputManager, int index);
        void RenderModuleWindow() override;
        ImTextureID GetTextureID();
};