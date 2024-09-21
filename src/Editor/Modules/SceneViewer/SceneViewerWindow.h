#pragma once

#include "pch.h"
#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.h"
#include "Core/Rendering/Pipelines/IRenderingPipeline.h"
#include "Core/Scene/Camera/ICamera.h"
#include "Core/Scene/Camera/ICameraFactory.h"
#include "Editor/Selection/IEditorSelection.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "Editor/Stats/IEditorStats.h"
#include "Input/IInputManager.h"
#include <boost/serialization/strong_typedef.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <imgui_internal.h>
#include <ImGuizmo.h>

#include "Editor/Modules/IGuiModule.h"
#include "Core/Rendering/Pipelines/IRenderingPipelineFactory.h"

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
    /// @brief Selects the constraints for the render target. 0 = no
    /// constraints, 1 = constrained by given aspect ratio, 2 = constrained by a
    /// fixed resolution.
    RENDERING_CONSTRAINT RenderingConstraint = RENDERING_CONSTRAINT::FREE;

    /// @brief Stored aspect ratio constraint.
    std::array<int, 2> AspectRatio = { 16, 9 };

    /// @brief Stored resolution constraint.
    std::array<int, 2> Resolution = { 1024, 1024 };

    /// @brief Aspect ratio to use for the target.
    float targetAspectRatio;

    /// @brief Currently available resolution for rendering the scene in the
    /// scene viewer window.
    glm::ivec2 ViewportSize;

    bool CameraMovement = false;

    bool RenderGrid = true;
  };

  /// @brief Module to render a window that displays the scene and render
  /// options.
  class SceneViewerWindow : public IGuiModule
  {
  private:
    SceneViewerSettings m_Settings;
    // Maintaining important dependencies

    /// @brief The render texture for this scene viewer.
    std::shared_ptr<IFramebuffer> m_Framebuffer;
    std::shared_ptr<IFramebuffer> m_IdBuffer;
    std::shared_ptr<IFramebuffer> m_OutlineBuffer;
    std::shared_ptr<IFramebuffer> m_PresentationBuffer;
    std::shared_ptr<ICamera>      m_Camera;

    std::shared_ptr<IInputManager>             m_InputManager;
    std::shared_ptr<IEditorStats>              m_EditorStats;
    std::shared_ptr<ILoadedScene>              m_LoadedScene;
    std::shared_ptr<IEditorSelection>          m_EditorSelection;
    std::shared_ptr<IRenderingPipelineFactory> m_RenderingPipelineFactory;
    std::shared_ptr<IRenderingPipeline>        m_RenderingPipeline;
    std::shared_ptr<IFramebufferFactory>       m_FramebufferFactory;
    std::shared_ptr<ICameraFactory>            m_CameraFactory;

    /// @brief Calculates the cutout of the available resolution based on the
    /// given aspect ratio.
    /// @param availableResolution Base resolution given.
    /// @param targetAspectRatio Desired aspect ratio.
    /// @return Resolution at the given aspect ratio.
    glm::ivec2
    CalculateDesiredResolution(glm::ivec2 availableResolution,
                               float      targetAspectRatio) const;

    glm::vec3
    CalculateSelectionCenter() const;

    /// @brief Updates the render texture.
    void
    UpdateFramebuffer();

    /// @brief Renders the ImGuizmo gizmos.
    void
    RenderGizmos(ImVec2 minRect, ImVec2 maxRect);

    /// @brief Handles the shortcut inputs for changing the gizmo type.
    void
    UpdateGizmoType();

    void
    ProcessSceneClick(glm::vec2 const& mousePosition,
                      glm::vec2 const& viewportSize);

  public:
    SceneViewerWindow(
      std::shared_ptr<ICameraFactory>            cameraFactory,
      std::shared_ptr<IFramebufferFactory>       framebufferFactory,
      std::shared_ptr<IEditorStats>              editorStats,
      std::shared_ptr<IInputManager>             inputManager,
      std::shared_ptr<ILoadedScene>              loadedScene,
      std::shared_ptr<IEditorSelection>          editorSelection,
      std::shared_ptr<IRenderingPipelineFactory> renderingPipelineFactory);

    SceneViewerWindow(
      SerializedModule                           serializedModule,
      std::shared_ptr<ICameraFactory>            cameraFactory,
      std::shared_ptr<IFramebufferFactory>       framebufferFactory,
      std::shared_ptr<IEditorStats>              editorStats,
      std::shared_ptr<IInputManager>             inputManager,
      std::shared_ptr<ILoadedScene>              loadedScene,
      std::shared_ptr<IEditorSelection>          editorSelection,
      std::shared_ptr<IRenderingPipelineFactory> renderingPipelineFactory);

    void
    OnUpdate() override;
    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    /// @brief Returns the frame buffer of the scene viewer as an IMGUI texture
    /// ID:
    /// @return The texture ID of the frame buffer.
    ImTextureID
    GetFrameBufferForImGui();

    nlohmann::json
    Serialize() const override;

    void
    Deserialize(nlohmann::json moduleData) override;
  };
}