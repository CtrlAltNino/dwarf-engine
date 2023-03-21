#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <imgui_internal.h>
#include <ImGuizmo.h>

#include "Core/Base.h"
#include "Editor/EditorModel.h"
#include "Editor/Modules/GuiModule.h"
#include "Core/Scene/Camera.h"
#include "Core/Rendering/Framebuffer.h"

namespace Dwarf {

    #define MIN_RESOLUTION_WIDTH 10
    #define MIN_RESOLUTION_HEIGHT 10
    #define MAX_RESOLUTION_WIDTH 5120
    #define MAX_RESOLUTION_HEIGHT 2160

    struct SceneViewerSettings {
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
        int cResolution[2] = {1024, 1024};

        /// @brief Aspect ratio to use for the target.
        float targetAspectRatio;

        /// @brief Currently available resolution for rendering the scene in the scene viewer window.
        glm::ivec2 availableResolution;
    };

    /// @brief Module to render a window that displays the scene and render options.
    class SceneViewerWindow : public GuiModule{
        private:
            SceneViewerSettings m_Settings;
            // Maintaining important dependencies
            
            /// @brief The render texture for this scene viewer.
            //Ref<IRenderTexture> renderTexture;
            Ref<Framebuffer> m_Framebuffer;

            Ref<Camera> m_Camera;

            // Resolution memory
            

            /// @brief Calculates the cutout of the available resolution based on the given aspect ratio.
            /// @param availableResolution Base resolution given.
            /// @param targetAspectRatio Desired aspect ratio.
            /// @return Resolution at the given aspect ratio.
            glm::ivec2 CalculateDesiredResolution(glm::ivec2 availableResolution, float targetAspectRatio);

            glm::vec3 CalculateSelectionCenter();

            /// @brief Updates the render texture.
            void UpdateFramebuffer();

            /// @brief Renders the ImGuizmo gizmos.
            void RenderGizmos();

            /// @brief Handles the shortcut inputs for changing the gizmo type.
            void UpdateGizmoType();
        public:
            SceneViewerWindow(Ref<EditorModel> model, int index);

            void OnUpdate(double deltaTime) override;
            /// @brief Renders the module window.
            void OnImGuiRender() override;

            /// @brief Returns the scene viewers render texture as a IMGUI texture ID:
            /// @return The texture ID of the render texture.
            ImTextureID GetTextureID();
    };
}