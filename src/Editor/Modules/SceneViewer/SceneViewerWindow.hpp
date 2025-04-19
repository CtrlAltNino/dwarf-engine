#pragma once

#include "Core/Rendering/Pipelines/IRenderingPipeline.hpp"
#include "Core/Rendering/Pipelines/IRenderingPipelineFactory.hpp"
#include "Core/Scene/Camera/ICamera.hpp"
#include "Core/Scene/Camera/ICameraFactory.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "Editor/Modules/IGuiModule.hpp"
#include "Editor/Selection/IEditorSelection.hpp"
#include "Editor/Stats/IEditorStats.hpp"
#include "Input/IInputManager.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "Window/IWindow.hpp"
#include <ImGuizmo.h>
#include <boost/serialization/strong_typedef.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Dwarf
{
  enum class RENDERING_CONSTRAINT : uint8_t
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
    glm::ivec2 AspectRatio = { 16, 9 };

    /// @brief Stored resolution constraint.
    glm::ivec2 Resolution = { 1024, 1024 };

    GridSettings GridSettings;

    /// @brief Aspect ratio to use for the target.
    float targetAspectRatio;

    /// @brief Currently available resolution for rendering the scene in the
    /// scene viewer window.
    glm::ivec2 ViewportSize;
    bool       CameraMovement = false;
  };

  /// @brief Module to render a window that displays the scene and render
  /// options.
  class SceneViewerWindow : public IGuiModule
  {
  private:
    SceneViewerSettings mSettings;
    // Maintaining important dependencies

    /// @brief The render texture for this scene viewer.
    std::shared_ptr<IDwarfLogger>       mLogger;
    std::unique_ptr<ICamera>            mCamera;
    std::shared_ptr<IRenderingPipeline> mRenderingPipeline;
    std::shared_ptr<IInputManager>      mInputManager;
    std::shared_ptr<IEditorStats>       mEditorStats;
    std::shared_ptr<ILoadedScene>       mLoadedScene;
    std::shared_ptr<IEditorSelection>   mEditorSelection;
    std::shared_ptr<IWindow>            mWindow;

    /// @brief Calculates the cutout of the available resolution based on the
    /// given aspect ratio.
    /// @param availableResolution Base resolution given.
    /// @param targetAspectRatio Desired aspect ratio.
    /// @return Resolution at the given aspect ratio.
    [[nodiscard]] static auto
    CalculateDesiredResolution(glm::ivec2 availableResolution,
                               float      targetAspectRatio) -> glm::ivec2;

    [[nodiscard]] auto
    CalculateSelectionCenter() const -> glm::vec3;

    /// @brief Updates the render texture.
    void
    UpdateFramebuffer();

    /// @brief Renders the ImGuizmo gizmos.
    void
    RenderGizmos(ImVec2 minRect, ImVec2 maxRect);

    /// @brief Handles the shortcut inputs for changing the gizmo type.
    void
    UpdateGizmoType();

    /**
     * @brief Processes a scene click at a given position in the id framebuffer.
     *
     * @param mousePosition
     */
    void
    ProcessSceneClick(glm::vec2 const& mousePosition);

    /**
     * @brief Renders the general scene viewer settings
     *
     */
    void
    RenderSceneViewerSettings();

    /**
     * @brief Renders the tool settings
     *
     */
    void
    RenderToolSettings();

    /**
     * @brief Renders the grid settings
     *
     */
    void
    RenderGridSettings();

    /**
     * @brief Renders the camera settings
     *
     */
    void
    RenderCameraSettings();

    /**
     * @brief Renderings the rendering settings
     *
     */
    void
    RenderRenderingSettings();

    /**
     * @brief Renders the statistics
     *
     */
    void
    RenderStats();

  public:
    SceneViewerWindow(std::shared_ptr<IDwarfLogger>          logger,
                      std::shared_ptr<IEditorStats>          editorStats,
                      std::shared_ptr<IInputManager>         inputManager,
                      std::shared_ptr<ILoadedScene>          loadedScene,
                      std::shared_ptr<IEditorSelection>      editorSelection,
                      const std::shared_ptr<ICameraFactory>& cameraFactory,
                      const std::shared_ptr<IRenderingPipelineFactory>&
                                               renderingPipelineFactory,
                      std::shared_ptr<IWindow> window);

    SceneViewerWindow(SerializedModule                       serializedModule,
                      std::shared_ptr<IDwarfLogger>          logger,
                      std::shared_ptr<IEditorStats>          editorStats,
                      std::shared_ptr<IInputManager>         inputManager,
                      std::shared_ptr<ILoadedScene>          loadedScene,
                      std::shared_ptr<IEditorSelection>      editorSelection,
                      const std::shared_ptr<ICameraFactory>& cameraFactory,
                      const std::shared_ptr<IRenderingPipelineFactory>&
                                               renderingPipelineFactory,
                      std::shared_ptr<IWindow> window);

    ~SceneViewerWindow() override;

    void
    OnUpdate() override;
    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    auto
    Serialize() -> nlohmann::json override;

    void
    Deserialize(nlohmann::json moduleData);
  };
}