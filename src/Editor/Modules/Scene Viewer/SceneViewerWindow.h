#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <imgui_internal.h>
#include <ImGuizmo.h>
#include <array>

#include "Core/Base.h"
#include "Editor/EditorModel.h"
#include "Editor/Modules/GuiModule.h"
#include "Core/Scene/Camera.h"
#include "Core/Rendering/Framebuffer.h"
#include "Core/Rendering/Forward/ForwardRenderer.h"
#include "Core/Rendering/ComputeShader.h"

namespace Dwarf
{
    enum class RENDERING_CONSTRAINT
    {
        FREE,
        ASPECT_RATIO,
        FIXED_RESOLUTION
    };

    struct SceneViewerSettings
    {
        /// @brief Currently selected object manipulation type.
        ImGuizmo::OPERATION GizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

        /// @brief Currently selected object manipulation space.
        ImGuizmo::MODE GizmoMode = ImGuizmo::MODE::LOCAL;

        // GUI exposed values
        /// @brief Selects the constraints for the render target. 0 = no constraints, 1 = constrained by given aspect ratio, 2 = constrained by a fixed resolution.
        RENDERING_CONSTRAINT RenderingConstraint = RENDERING_CONSTRAINT::FREE;

        /// @brief Stored aspect ratio constraint.
        std::array<int, 2> AspectRatio = {16, 9};

        /// @brief Stored resolution constraint.
        std::array<int, 2> Resolution = {1024, 1024};

        /// @brief Aspect ratio to use for the target.
        float targetAspectRatio;

        /// @brief Currently available resolution for rendering the scene in the scene viewer window.
        glm::ivec2 ViewportSize;

        bool CameraMovement = false;

        bool RenderGrid = true;
    };

    /// @brief Module to render a window that displays the scene and render options.
    class SceneViewerWindow : public GuiModule
    {
    private:
        SceneViewerSettings m_Settings;
        // Maintaining important dependencies

        /// @brief The render texture for this scene viewer.
        Ref<Framebuffer> m_Framebuffer;

        Ref<Framebuffer> m_IdBuffer;

        Ref<Framebuffer> m_OutlineBuffer;

        Ref<Camera> m_Camera;

        /// @brief Calculates the cutout of the available resolution based on the given aspect ratio.
        /// @param availableResolution Base resolution given.
        /// @param targetAspectRatio Desired aspect ratio.
        /// @return Resolution at the given aspect ratio.
        glm::ivec2 CalculateDesiredResolution(glm::ivec2 availableResolution, float targetAspectRatio) const;

        glm::vec3 CalculateSelectionCenter() const;

        /// @brief Updates the render texture.
        void UpdateFramebuffer();

        /// @brief Renders the ImGuizmo gizmos.
        void RenderGizmos(ImVec2 minRect, ImVec2 maxRect);

        /// @brief Handles the shortcut inputs for changing the gizmo type.
        void UpdateGizmoType();

        void ProcessSceneClick(glm::vec2 const &mousePosition, glm::vec2 const &viewportSize);

        glm::vec3 GetRayDirection(float mouseX, float mouseY, int screenWidth, int screenHeight, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);

        bool RayIntersectsMesh(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, const glm::mat4 &modelMatrix, Ref<Mesh> mesh, float &intersectionDistance);

    public:
        SceneViewerWindow(Ref<EditorModel> model, int index);

        void OnUpdate(double deltaTime) override;
        /// @brief Renders the module window.
        void OnImGuiRender() override;

        /// @brief Returns the frame buffer of the scene viewer as an IMGUI texture ID:
        /// @return The texture ID of the frame buffer.
        ImTextureID GetFrameBufferForImGui();

        std::string Serialize() override;

        void Deserialize(nlohmann::json moduleData) override;
    };
}