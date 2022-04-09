#pragma once

#include "../../../utilities/dpch.h"

#include<imgui_internal.h>
#include<glm/vec2.hpp>
#include<glm/vec3.hpp>

#include"../../editor/EditorProperties.h"
#include "../IModule.h"
#include"../../rendering/IRenderTexture.h"
#include"../../scene/Camera.h"
#include"../../data structures/Quaternion.h"
#include"../../input/IInputManager.h"

#define MIN_RESOLUTION_WIDTH 10
#define MIN_RESOLUTION_HEIGHT 10
#define MAX_RESOLUTION_WIDTH 5120
#define MAX_RESOLUTION_HEIGHT 2160

class SceneViewerModule : public IModule{
    private:
        // Maintaining important dependencies
        IRenderTexture* renderTexture;
        IInputManager* inputManager;
        IWindowManager* windowManager;

        // GUI exposed values
        int selectedRenderingMode = 0;
        int cAspectRatio[2] = {16, 9};
        int cResolution[2] = {renderTexture->GetResolution().x, renderTexture->GetResolution().y};

        // Camera relevant values
        bool cameraUpdating = false;
        glm::vec2 lastMousePos = glm::vec2(-1);
        glm::vec2 deltaMousePos = glm::vec2(0);

        // Resolution memory
        glm::ivec2 availableResolution;
        float targetAspectRatio;
        
        glm::ivec2 CalculateDesiredResolution(glm::ivec2 availableResolution, float targetAspectRatio);
        void UpdateCamera();
        void UpdateRenderTexture();
    public:
        SceneViewerModule(IViewListener *listener, IRenderTexture* renderTexture, IInputManager* inputManager, int index);
        void RenderModuleWindow() override;
        ImTextureID GetTextureID();
        void StartFrame() override;
};