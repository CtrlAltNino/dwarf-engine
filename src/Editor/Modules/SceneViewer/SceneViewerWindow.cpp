#include "pch.h"

#include "Core/Scene/Components/SceneComponents.h"
#include "Core/Scene/Entity/Entity.h"
#include "Editor/Modules/SceneViewer/SceneViewerWindow.h"
#include <imgui.h>

#define MIN_RESOLUTION_WIDTH 10
#define MIN_RESOLUTION_HEIGHT 10
#define MAX_RESOLUTION_WIDTH 5120
#define MAX_RESOLUTION_HEIGHT 2160

namespace Dwarf
{
  SceneViewerWindow::SceneViewerWindow(
    std::shared_ptr<IDwarfLogger>                     logger,
    std::shared_ptr<IEditorStats>                     editorStats,
    std::shared_ptr<IInputManager>                    inputManager,
    std::shared_ptr<ILoadedScene>                     loadedScene,
    std::shared_ptr<IEditorSelection>                 editorSelection,
    const std::shared_ptr<ICameraFactory>&            cameraFactory,
    const std::shared_ptr<IRenderingPipelineFactory>& renderingPipelineFactory,
    std::shared_ptr<IWindow>                          window)
    : IGuiModule(ModuleLabel("Scene Viewer"),
                 ModuleType(MODULE_TYPE::SCENE_VIEWER),
                 ModuleID(std::make_shared<UUID>()))
    , mLogger(std::move(logger))
    , mEditorStats(std::move(editorStats))
    , mInputManager(std::move(inputManager))
    , mLoadedScene(std::move(loadedScene))
    , mEditorSelection(std::move(editorSelection))
    , mWindow(std::move(window))
  {
    // Create rendering pipeline
    mRenderingPipeline = renderingPipelineFactory->Create();
    mSettings.MaxSamples = mRenderingPipeline->GetMaxMsaaSamples();

    // Setup camera
    mCamera = cameraFactory->Create();

    mLogger->LogDebug(Log("SceneViewerWindow created", "SceneViewerWindow"));
  }

  SceneViewerWindow::SceneViewerWindow(
    SerializedModule                                  serializedModule,
    std::shared_ptr<IDwarfLogger>                     logger,
    std::shared_ptr<IEditorStats>                     editorStats,
    std::shared_ptr<IInputManager>                    inputManager,
    std::shared_ptr<ILoadedScene>                     loadedScene,
    std::shared_ptr<IEditorSelection>                 editorSelection,
    const std::shared_ptr<ICameraFactory>&            cameraFactory,
    const std::shared_ptr<IRenderingPipelineFactory>& renderingPipelineFactory,
    std::shared_ptr<IWindow>                          window)
    : IGuiModule(ModuleLabel("Scene Viewer"),
                 ModuleType(MODULE_TYPE::SCENE_VIEWER),
                 ModuleID(std::make_shared<UUID>(
                   serializedModule.t["id"].get<std::string>())))
    , mLogger(std::move(logger))
    , mEditorStats(std::move(editorStats))
    , mInputManager(std::move(inputManager))
    , mLoadedScene(std::move(loadedScene))
    , mEditorSelection(std::move(editorSelection))
    , mWindow(std::move(window))
  {
    // Create rendering pipeline
    mRenderingPipeline = renderingPipelineFactory->Create();
    mSettings.MaxSamples = mRenderingPipeline->GetMaxMsaaSamples();

    // Setup camera
    mCamera = cameraFactory->Create(serializedModule.t["camera"]);

    Deserialize(serializedModule.t);

    mLogger->LogDebug(Log("SceneViewerWindow created", "SceneViewerWindow"));
  }

  SceneViewerWindow::~SceneViewerWindow()
  {
    mLogger->LogDebug(Log("SceneViewerWindow destroyed", "SceneViewerWindow"));
  }

  void
  SceneViewerWindow::OnUpdate()
  {
    if (mSettings.CameraMovement &&
        mInputManager->GetMouseButton(MOUSE_BUTTON::RIGHT))
    {
      mWindow->SetMouseVisibility(false);
      mCamera->OnUpdate(mEditorStats->GetDeltaTime());
    }
    else
    {
      mWindow->SetMouseVisibility(true);
    }

    mRenderingPipeline->SetExposue(mSettings.Exposure);

    // Render scene to the framebuffer with the camera
    mRenderingPipeline->RenderScene(*mCamera, mSettings.RenderGrid);

    mRenderingPipeline->RenderIds(mLoadedScene->GetScene(), *mCamera);
  }

  void
  SceneViewerWindow::OnImGuiRender()
  {
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoCollapse;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(800, 500));

    ImGui::Begin(GetIdentifier().c_str(), &mWindowOpened, windowFlags);

    ImGui::PopStyleVar(2);
    static std::array<std::string, 3> renderingModes = { "Free",
                                                         "Aspect Ratio",
                                                         "Fixed Resolution" };

    ImGui::PushItemWidth(150);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    // Rendering mode combo
    if (ImGui::BeginCombo(
          "##renderingMode",
          renderingModes[(int)mSettings.RenderingConstraint].c_str()))
    {
      for (int n = 0; n < 3; n++)
      {
        const bool is_selected = ((int)mSettings.RenderingConstraint == n);

        // ==================== Graphics Selectable ====================
        if (ImGui::Selectable(
              renderingModes[n].c_str(), is_selected, 0, ImVec2(0, 16 + 10)))
        {
          mSettings.RenderingConstraint = (RENDERING_CONSTRAINT)n;
        }

        // Set the initial focus when opening the combo (scrolling + keyboard
        // navigation focus)
        if (is_selected)
        {
          ImGui::SetItemDefaultFocus();
        }
      }

      ImGui::EndCombo();
    }

    ImGui::SameLine(0, 5);

    // Guizmo model toggle
    if (mSettings.GizmoMode == ImGuizmo::MODE::LOCAL)
    {
      if (ImGui::Button("Local"))
      {
        mSettings.GizmoMode = ImGuizmo::MODE::WORLD;
      }
    }
    else
    {
      if (ImGui::Button("World"))
      {
        mSettings.GizmoMode = ImGuizmo::MODE::LOCAL;
      }
    }

    ImGui::SameLine(0, 5);

    // Guizmo type translate button
    if (ImGui::Button("Translate"))
    {
      mSettings.GizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
    }

    ImGui::SameLine(0, 5);

    // Guizmo type rotate button
    if (ImGui::Button("Rotate"))
    {
      mSettings.GizmoOperation = ImGuizmo::OPERATION::ROTATE;
    }

    ImGui::SameLine(0, 5);

    // Guizmo type scale button
    if (ImGui::Button("Scale"))
    {
      mSettings.GizmoOperation = ImGuizmo::OPERATION::SCALE;
    }

    // Rendering mode specific settings input

    if (mSettings.RenderingConstraint == RENDERING_CONSTRAINT::ASPECT_RATIO)
    {
      ImGui::SameLine(0, 10);
      ImGui::Text("Aspect ratio");

      ImGui::SameLine(0, 10);
      ImGui::InputInt("##aspectWidth", mSettings.AspectRatio.data());
      ImGui::SameLine(0, 10);
      ImGui::InputInt("##aspectHeight", &(mSettings.AspectRatio[1]));

      mSettings.AspectRatio[0] =
        std::max(1, std::min(MAX_RESOLUTION_WIDTH, mSettings.AspectRatio[0]));
      mSettings.AspectRatio[1] =
        std::max(1, std::min(MAX_RESOLUTION_HEIGHT, mSettings.AspectRatio[1]));
    }
    else if (mSettings.RenderingConstraint ==
             RENDERING_CONSTRAINT::FIXED_RESOLUTION)
    {
      ImGui::SameLine(0, 10);
      ImGui::Text("Resolution");
      ImGui::SameLine(0, 10);
      ImGui::InputInt2("", mSettings.Resolution.data());
      mSettings.Resolution[0] =
        std::max(MIN_RESOLUTION_WIDTH,
                 std::min(MAX_RESOLUTION_WIDTH, mSettings.Resolution[0]));
      mSettings.Resolution[1] =
        std::max(MIN_RESOLUTION_WIDTH,
                 std::min(MAX_RESOLUTION_WIDTH, mSettings.Resolution[1]));
    }

    ImGui::SameLine(0, 5);

    ImGui::Checkbox("Render Grid", &mSettings.RenderGrid);

    ImGui::SameLine(0, 5);

    ImGui::SliderInt("MSAA Samples",
                     &mSettings.Samples,
                     1,
                     mSettings.MaxSamples,
                     "%d",
                     ImGuiSliderFlags_None);

    ImGui::SameLine(0, 5);

    ImGui::DragFloat("Exposure", &mSettings.Exposure, 0.0001F, 0, 2);

    ImGui::PopStyleVar();
    ImGui::PopItemWidth();

    // Calculating the rect size for the framebuffer
    ImVec2 minRect = ImGui::GetCursorScreenPos();
    ImVec2 maxRect(
      ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
      ImGui::GetCursorScreenPos().y + ImGui::GetContentRegionAvail().y);

    mSettings.ViewportSize = glm::ivec2(ImGui::GetContentRegionAvail().x,
                                        ImGui::GetContentRegionAvail().y);

    UpdateFramebuffer();

    if (mSettings.RenderingConstraint != RENDERING_CONSTRAINT::FREE)
    {
      if (((float)mSettings.ViewportSize.x / (float)mSettings.ViewportSize.y) <
          mSettings.targetAspectRatio)
      {
        float diff = std::ceil(std::fabs((float)mSettings.ViewportSize.y -
                                         ((float)mSettings.ViewportSize.x /
                                          mSettings.targetAspectRatio)) /
                               2.0F);
        minRect.y += diff;
        maxRect.y -= diff;
      }
      else if (((double)mSettings.ViewportSize.x /
                (double)mSettings.ViewportSize.y) > mSettings.targetAspectRatio)
      {
        float diff = std::ceil(std::fabs((float)mSettings.ViewportSize.x -
                                         ((float)mSettings.ViewportSize.y *
                                          mSettings.targetAspectRatio)) /
                               2.0F);
        minRect.x += diff;
        maxRect.x -= diff;
      }
    }

    ImGui::SetCursorScreenPos(minRect);
    ImVec2 size = { maxRect.x - minRect.x, maxRect.y - minRect.y };

    // Rendering the framebuffer
    ImGui::Image((ImTextureID)mRenderingPipeline->GetPresentationBufferId(),
                 size,
                 ImVec2(0, 1),
                 ImVec2(1, 0));

    // Check if a mesh is clicked
    glm::vec2 mousePos = { ImGui::GetMousePos().x, ImGui::GetMousePos().y };
    glm::vec2 minRectGlm = { minRect.x, minRect.y };
    glm::vec2 maxRectGlm = { maxRect.x, maxRect.y };

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
      ProcessSceneClick(mousePos - minRectGlm);
    }

    if (!mSettings.CameraMovement && ImGui::IsItemHovered() &&
        mInputManager->GetMouseButtonDown(MOUSE_BUTTON::RIGHT))
    {
      bool isWindowFocused = ImGui::IsWindowFocused();
      mSettings.CameraMovement = true;
      ImGui::FocusWindow(ImGui::GetCurrentWindow());
    }
    else if (mSettings.CameraMovement &&
             !mInputManager->GetMouseButton(MOUSE_BUTTON::RIGHT))
    {
      mSettings.CameraMovement = false;
    }

    if (!mEditorSelection->GetSelectedEntities().empty())
    {
      RenderGizmos(minRect, maxRect);
    }
    ImGui::End();
  }

  auto
  SceneViewerWindow::CalculateDesiredResolution(glm::ivec2 availableResolution,
                                                float      targetAspectRatio)
    -> glm::ivec2
  {
    glm::ivec2 desiredResolution = availableResolution;

    if (((double)availableResolution.x / (double)availableResolution.y) <
        targetAspectRatio)
    {
      desiredResolution.y =
        (int)std::ceil((float)availableResolution.x / targetAspectRatio);
    }
    else if (((double)availableResolution.x / (double)availableResolution.y) >
             targetAspectRatio)
    {
      desiredResolution.x =
        (int)std::ceil((float)availableResolution.y * targetAspectRatio);
    }

    return desiredResolution;
  }

  void
  SceneViewerWindow::RenderGizmos(ImVec2 minRect, ImVec2 maxRect)
  {
    UpdateGizmoType();
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();

    ImGuizmo::SetRect(
      minRect.x, minRect.y, maxRect.x - minRect.x, maxRect.y - minRect.y);

    Entity    entity(mEditorSelection->GetSelectedEntities().at(0),
                  mLoadedScene->GetScene().GetRegistry());
    auto&     transformComponent = entity.GetComponent<TransformComponent>();
    glm::mat4 transform = transformComponent.GetModelMatrix();

    ImGuizmo::Manipulate(glm::value_ptr(mCamera->GetViewMatrix()),
                         glm::value_ptr(mCamera->GetProjectionMatrix()),
                         mSettings.GizmoOperation,
                         mSettings.GizmoMode,
                         glm::value_ptr(transform));
    if (ImGuizmo::IsUsing())
    {
      transformComponent.GetPosition() = glm::vec3(transform[3]);
      transformComponent.GetScale() =
        glm::vec3(glm::length(glm::vec3(transform[0])),
                  glm::length(glm::vec3(transform[1])),
                  glm::length(glm::vec3(transform[2])));

      auto rotationMatrix =
        glm::mat3(transform); // Remove translation from matrix
      rotationMatrix = glm::mat3(glm::normalize(rotationMatrix[0]),
                                 glm::normalize(rotationMatrix[1]),
                                 glm::normalize(rotationMatrix[2]));

      // Convert rotation matrix to Euler angles (in radians)
      glm::quat rotationQuat(rotationMatrix);
      transformComponent.GetEulerAngles() = glm::degrees(
        glm::eulerAngles(rotationQuat)); // Convert to Euler angles in degrees
    }
  }

  void
  SceneViewerWindow::UpdateGizmoType()
  {
    if (ImGui::IsWindowFocused() &&
        !mInputManager->GetMouseButton(MOUSE_BUTTON::RIGHT))
    {
      using enum KEYCODE;
      if (mInputManager->GetKeyDown(W))
      {
        mSettings.GizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
      }
      else if (mInputManager->GetKeyDown(E))
      {
        mSettings.GizmoOperation = ImGuizmo::OPERATION::ROTATE;
      }
      else if (mInputManager->GetKeyDown(R))
      {
        mSettings.GizmoOperation = ImGuizmo::OPERATION::SCALE;
      }
    }
  }

  auto
  SceneViewerWindow::CalculateSelectionCenter() const -> glm::vec3
  {
    auto center = glm::vec3(0);

    for (auto handle : mEditorSelection->GetSelectedEntities())
    {
      Entity entity(handle, mLoadedScene->GetScene().GetRegistry());
      center += entity.GetComponent<TransformComponent>().GetPosition();
    }

    return center / (float)mEditorSelection->GetSelectedEntities().size();
  }

  void
  SceneViewerWindow::UpdateFramebuffer()
  {
    glm::ivec2 desiredResolution;

    switch (mSettings.RenderingConstraint)
    {
      case RENDERING_CONSTRAINT::FREE:
        {
          desiredResolution = mSettings.ViewportSize;
          mSettings.targetAspectRatio =
            (float)desiredResolution.x / (float)desiredResolution.y;
        }
        break;
      case RENDERING_CONSTRAINT::FIXED_RESOLUTION:
        {
          desiredResolution =
            glm::ivec2(mSettings.Resolution[0], mSettings.Resolution[1]);
          mSettings.targetAspectRatio =
            (float)mSettings.Resolution[0] / (float)mSettings.Resolution[1];
        }
        break;
      case RENDERING_CONSTRAINT::ASPECT_RATIO:
        {
          desiredResolution = CalculateDesiredResolution(
            mSettings.ViewportSize,
            (float)mSettings.AspectRatio[0] / (float)mSettings.AspectRatio[1]);
          mSettings.targetAspectRatio =
            (float)desiredResolution.x / (float)desiredResolution.y;
        }
        break;
    }

    if (mRenderingPipeline->GetResolution() != desiredResolution)
    {
      mRenderingPipeline->SetResolution(desiredResolution);
      mCamera->GetProperties().AspectRatio =
        (float)desiredResolution.x / (float)desiredResolution.y;
    }

    if (mRenderingPipeline->GetMsaaSamples() != mSettings.Samples)
    {
      mRenderingPipeline->SetMsaaSamples(mSettings.Samples);
    }
  }

  void
  SceneViewerWindow::Deserialize(nlohmann::json moduleData)
  {
    mSettings.AspectRatio[0] =
      moduleData["settings"]["aspectRatioConstraint"]["x"];
    mSettings.AspectRatio[1] =
      moduleData["settings"]["aspectRatioConstraint"]["y"];

    mSettings.Resolution[0] =
      moduleData["settings"]["resolutionConstraint"]["x"];
    mSettings.Resolution[1] =
      moduleData["settings"]["resolutionConstraint"]["y"];

    mSettings.RenderingConstraint =
      (RENDERING_CONSTRAINT)moduleData["settings"]["constraintType"];

    mSettings.RenderGrid = moduleData["settings"]["renderGrid"];

    if (moduleData.contains("settings") &&
        moduleData["settings"].contains("exposure"))
    {
      mSettings.Exposure = moduleData["settings"]["exposure"].get<float>();
    }

    if (moduleData.contains("settings") &&
        moduleData["settings"].contains("samples"))
    {
      mSettings.Samples = std::min(
        moduleData["settings"]["samples"].get<int32_t>(), mSettings.MaxSamples);
    }
  }

  auto
  SceneViewerWindow::Serialize() -> nlohmann::json
  {
    nlohmann::json serializedModule;

    serializedModule["camera"] = mCamera->Serialize();

    serializedModule["settings"]["aspectRatioConstraint"]["x"] =
      mSettings.AspectRatio[0];
    serializedModule["settings"]["aspectRatioConstraint"]["y"] =
      mSettings.AspectRatio[1];

    serializedModule["settings"]["resolutionConstraint"]["x"] =
      mSettings.Resolution[0];
    serializedModule["settings"]["resolutionConstraint"]["y"] =
      mSettings.Resolution[1];

    serializedModule["settings"]["constraintType"] =
      mSettings.RenderingConstraint;

    serializedModule["settings"]["renderGrid"] = mSettings.RenderGrid;

    serializedModule["settings"]["samples"] = mSettings.Samples;
    serializedModule["settings"]["exposure"] = mSettings.Exposure;

    serializedModule["id"] = GetUuid()->toString();
    serializedModule["type"] = static_cast<int>(GetModuleType());
    serializedModule["label"] = GetModuleName();

    return serializedModule;
  }

  void
  SceneViewerWindow::ProcessSceneClick(glm::vec2 const& mousePosition)
  {
    uint32_t handle = mRenderingPipeline->ReadPixelId(mousePosition);

    if (handle > 0)
    {
      auto entity = static_cast<entt::entity>(handle);
      mEditorSelection->SelectEntity(entity);
    }
    else
    {
      mEditorSelection->ClearEntitySelection();
    }
  }
}