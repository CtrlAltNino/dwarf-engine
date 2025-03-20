#pragma once
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.h"
#include "pch.h"

#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.h"
#include "Core/Rendering/Pipelines/IRenderingPipeline.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"
#include "Core/Rendering/RendererApi/IRendererApiFactory.h"
#include "Core/Scene/Camera/ICamera.h"
#include "Core/Scene/Camera/ICameraFactory.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "Editor/Selection/IEditorSelection.h"
#include "Editor/Stats/IEditorStats.h"
#include "Input/IInputManager.h"
#include <ImGuizmo.h>
#include <boost/serialization/strong_typedef.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>

#include "Core/Rendering/Pipelines/IRenderingPipelineFactory.h"
#include "Editor/Modules/IGuiModule.h"

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

    int32_t MaxSamples = 1;

    int32_t Samples = 1;

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
    SceneViewerSettings mSettings;
    // Maintaining important dependencies

    /// @brief The render texture for this scene viewer.
    std::shared_ptr<IFramebuffer>       mFramebuffer;
    std::shared_ptr<IFramebuffer>       mIdBuffer;
    std::shared_ptr<IFramebuffer>       mOutlineBuffer;
    std::shared_ptr<IFramebuffer>       mPresentationBuffer;
    std::shared_ptr<ICamera>            mCamera;
    std::shared_ptr<IRenderingPipeline> mRenderingPipeline;
    std::shared_ptr<IRendererApi>       mRendererApi;
    std::shared_ptr<IShader>            mAgxTonemapShader;

    std::shared_ptr<IInputManager>             mInputManager;
    std::shared_ptr<IEditorStats>              mEditorStats;
    std::shared_ptr<ILoadedScene>              mLoadedScene;
    std::shared_ptr<IEditorSelection>          mEditorSelection;
    std::shared_ptr<IRenderingPipelineFactory> mRenderingPipelineFactory;
    std::shared_ptr<IFramebufferFactory>       mFramebufferFactory;
    std::shared_ptr<ICameraFactory>            mCameraFactory;
    std::shared_ptr<IRendererApiFactory>       mRendererApiFactory;
    std::shared_ptr<IShaderRegistry>           mShaderRegistry;
    std::shared_ptr<IShaderSourceCollectionFactory>
      mShaderSourceCollectionFactory;

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
      std::shared_ptr<IRenderingPipelineFactory> renderingPipelineFactory,
      std::shared_ptr<IRendererApiFactory>       rendererApiFactory,
      std::shared_ptr<IShaderRegistry>           shaderRegistry,
      std::shared_ptr<IShaderSourceCollectionFactory>
        shaderSourceCollectionFactory);

    SceneViewerWindow(
      SerializedModule                           serializedModule,
      std::shared_ptr<ICameraFactory>            cameraFactory,
      std::shared_ptr<IFramebufferFactory>       framebufferFactory,
      std::shared_ptr<IEditorStats>              editorStats,
      std::shared_ptr<IInputManager>             inputManager,
      std::shared_ptr<ILoadedScene>              loadedScene,
      std::shared_ptr<IEditorSelection>          editorSelection,
      std::shared_ptr<IRenderingPipelineFactory> renderingPipelineFactory,
      std::shared_ptr<IRendererApiFactory>       rendererApiFactory,
      std::shared_ptr<IShaderRegistry>           shaderRegistry,
      std::shared_ptr<IShaderSourceCollectionFactory>
        shaderSourceCollectionFactory);

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
    Serialize() override;

    void
    Deserialize(nlohmann::json moduleData);
  };
}