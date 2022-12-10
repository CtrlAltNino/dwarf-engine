#pragma once

#include "../../../utilities/dpch.h"

#include<imgui_internal.h>
#include<glm/vec2.hpp>
#include<glm/vec3.hpp>
#include<glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<ImGuizmo.h>

#include"../../editor/EditorProperties.h"
#include "../IModule.h"
#include"../../rendering/IRenderTexture.h"
#include"../../scene/Camera.h"
#include"../../scene/Scene.h"
//#include"../../data structures/Quaternion.h"
#include"../../input/IInputManager.h"

#define MIN_RESOLUTION_WIDTH 10
#define MIN_RESOLUTION_HEIGHT 10
#define MAX_RESOLUTION_WIDTH 5120
#define MAX_RESOLUTION_HEIGHT 2160

/// @brief Module to render a window that displays the scene and render options.
class SceneViewerModule : public IModule{
    private:
        // Maintaining important dependencies
        
        /// @brief The render texture for this scene viewer.
        IRenderTexture* renderTexture;

        /// @brief The input manager of the editor window.
        IInputManager* inputManager;

        /// @brief The window manager for the editor window.
        IWindowManager* windowManager;

        /// @brief Currently opened scene.
        Scene* scene;

        /// @brief Currently selected object manipulation type.
        ImGuizmo::OPERATION gizmoType = ImGuizmo::OPERATION::TRANSLATE;

        /// @brief Currently selected object manipulation space.
        ImGuizmo::MODE gizmoMode = ImGuizmo::MODE::LOCAL;

        // GUI exposed values
        /// @brief Selects the constraints for the render target. 0 = no constraints, 1 = constrained by given aspect ratio, 2 = constrained by a fixed resolution.
        int selectedRenderingMode = 0;

        /// @brief Stored aspect ratio constraint.
        int cAspectRatio[2] = {16, 9};

        /// @brief Stored resolution constraint.
        int cResolution[2] = {renderTexture->GetResolution().x, renderTexture->GetResolution().y};

        // Camera relevant values
        
        /// @brief Flag indicating if the user is currently navigating the camera.
        bool cameraUpdating = false;

        /// @brief Stores the mouse position of the last frame.
        glm::vec2 lastMousePos = glm::vec2(-1);

        /// @brief Stores the difference of the current frame, and last frame mouse position.
        glm::vec2 deltaMousePos = glm::vec2(0);

        // Resolution memory

        /// @brief Currently available resolution for rendering the scene in the scene viewer window.
        glm::ivec2 availableResolution;

        /// @brief Aspect ratio to use for the target.
        float targetAspectRatio;
        
        /// @brief Calculates the cutout of the available resolution based on the given aspect ratio.
        /// @param availableResolution Base resolution given.
        /// @param targetAspectRatio Desired aspect ratio.
        /// @return Resolution at the given aspect ratio.
        glm::ivec2 CalculateDesiredResolution(glm::ivec2 availableResolution, float targetAspectRatio);

        /// @brief Updates the camera of the scene viewer.
        void UpdateCamera();

        /// @brief Updates the render texture.
        void UpdateRenderTexture();

        /// @brief Renders the ImGuizmo gizmos.
        void RenderGizmos();

        /// @brief Handles the shortcut inputs for changing the gizmo type.
        void UpdateGizmoType();
    public:
        SceneViewerModule(IViewListener *listener, IRenderTexture* renderTexture, IInputManager* inputManager, Scene* scene, int index);

        /// @brief Renders the module window.
        void RenderModuleWindow() override;

        /// @brief Returns the scene viewers render texture as a IMGUI texture ID:
        /// @return The texture ID of the render texture.
        ImTextureID GetTextureID();

        /// @brief Executes all pre frame tasks.
        void StartFrame() override;
};