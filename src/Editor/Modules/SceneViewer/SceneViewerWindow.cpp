#include "Editor/Modules/SceneViewer/SceneViewerWindow.h"
#include "Core/Rendering/RendererApi/IRendererApiFactory.h"
#include "Core/Scene/Components/SceneComponents.h"
#include "Core/Scene/Entity/Entity.h"
#include <iostream>

#define MIN_RESOLUTION_WIDTH 10
#define MIN_RESOLUTION_HEIGHT 10
#define MAX_RESOLUTION_WIDTH 5120
#define MAX_RESOLUTION_HEIGHT 2160

namespace Dwarf
{
  SceneViewerWindow::SceneViewerWindow(
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
      shaderSourceCollectionFactory)
    : IGuiModule(ModuleLabel("Scene Viewer"),
                 ModuleType(MODULE_TYPE::SCENE_VIEWER),
                 ModuleID(std::make_shared<UUID>()))
    , mCameraFactory(cameraFactory)
    , mFramebufferFactory(framebufferFactory)
    , mEditorStats(editorStats)
    , mInputManager(inputManager)
    , mLoadedScene(loadedScene)
    , mEditorSelection(editorSelection)
    , mRenderingPipelineFactory(renderingPipelineFactory)
    , mRendererApiFactory(rendererApiFactory)
    , mShaderRegistry(shaderRegistry)
    , mShaderSourceCollectionFactory(shaderSourceCollectionFactory)
  {
    mRendererApi = mRendererApiFactory->Create();
    mSettings.MaxSamples = mRendererApi->GetMaxSamples();

    // Create rendering pipeline
    mRenderingPipeline =
      mRenderingPipelineFactory->Create(PipelineType::Forward);

    mAgxTonemapShader = mShaderRegistry->GetOrCreate(
      mShaderSourceCollectionFactory->CreateAgxTonemapShaderSourceCollection());
    mAgxTonemapShader->Compile();

    // Setup rendering framebuffer according to the pipeline specification
    mFramebuffer =
      mFramebufferFactory->Create(mRenderingPipeline->GetSpecification());

    // Setup framebuffer for presentation
    FramebufferSpecification presentationSpec;
    presentationSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RGBA8 }
    };

    mPresentationBuffer = mFramebufferFactory->Create(presentationSpec);

    // Setup frame buffer for rendering ids
    FramebufferSpecification idSpec;
    idSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RED_INTEGER },
      FramebufferTextureSpecification{ FramebufferTextureFormat::DEPTH }
    };
    idSpec.Width = 512;
    idSpec.Height = 512;
    mIdBuffer = mFramebufferFactory->Create(idSpec);

    // Setup outline buffer

    // Setup camera
    mCamera = mCameraFactory->Create();
  }

  SceneViewerWindow::SceneViewerWindow(
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
      shaderSourceCollectionFactory)
    : IGuiModule(ModuleLabel("Scene Viewer"),
                 ModuleType(MODULE_TYPE::SCENE_VIEWER),
                 ModuleID(std::make_shared<UUID>(
                   serializedModule.t["id"].get<std::string>())))
    , mCameraFactory(cameraFactory)
    , mFramebufferFactory(framebufferFactory)
    , mEditorStats(editorStats)
    , mInputManager(inputManager)
    , mLoadedScene(loadedScene)
    , mEditorSelection(editorSelection)
    , mRenderingPipelineFactory(renderingPipelineFactory)
    , mRendererApiFactory(rendererApiFactory)
    , mShaderRegistry(shaderRegistry)
    , mShaderSourceCollectionFactory(shaderSourceCollectionFactory)
  {
    mRendererApi = mRendererApiFactory->Create();
    mSettings.MaxSamples = mRendererApi->GetMaxSamples();

    // Create rendering pipeline
    mRenderingPipeline =
      mRenderingPipelineFactory->Create(PipelineType::Forward);

    mAgxTonemapShader = mShaderRegistry->GetOrCreate(
      mShaderSourceCollectionFactory->CreateAgxTonemapShaderSourceCollection());
    mAgxTonemapShader->Compile();

    // Setup rendering framebuffer according to the pipeline specification
    mFramebuffer =
      mFramebufferFactory->Create(mRenderingPipeline->GetSpecification());

    // Setup framebuffer for presentation
    FramebufferSpecification presentationSpec;
    presentationSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RGBA8 }
    };

    mPresentationBuffer = mFramebufferFactory->Create(presentationSpec);

    // Setup frame buffer for rendering ids
    FramebufferSpecification idSpec;
    idSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RED_INTEGER },
      FramebufferTextureSpecification{ FramebufferTextureFormat::DEPTH }
    };
    idSpec.Width = 512;
    idSpec.Height = 512;
    mIdBuffer = mFramebufferFactory->Create(idSpec);

    // Setup outline buffer

    // Setup camera
    mCamera = mCameraFactory->Create(serializedModule.t["camera"]);

    Deserialize(serializedModule.t);
  }

  void
  SceneViewerWindow::OnUpdate()
  {
    if (mSettings.CameraMovement &&
        mInputManager->GetMouseButton(MOUSE_BUTTON::RIGHT))
    {
      mCamera->OnUpdate(mEditorStats->GetDeltaTime());
    }

    // Render scene to the framebuffer with the camera
    mFramebuffer->Bind();
    mRenderingPipeline->RenderScene(mLoadedScene->GetScene(),
                                    *mCamera,
                                    { mFramebuffer->GetSpecification().Width,
                                      mFramebuffer->GetSpecification().Height },
                                    mSettings.RenderGrid);
    mFramebuffer->Unbind();

    mIdBuffer->Bind();
    mIdBuffer->ClearAttachment(0, 0);
    mRenderingPipeline->RenderIds(mLoadedScene->GetScene(),
                                  *mCamera,
                                  { mIdBuffer->GetSpecification().Width,
                                    mIdBuffer->GetSpecification().Height });
    mIdBuffer->Unbind();

    /*if (mModel->GetSelection().GetSelectedEntities().size() > 0)
    {
      mOutlineBuffer->Clear(glm::vec4(0));
      mOutlineBuffer->Bind();
      for (auto entity : mModel->GetSelection().GetSelectedEntities())
      {
        Renderer::Get()->RenderEntity(entity,
                                      mCamera->GetViewMatrix(),
                                      mCamera->GetProjectionMatrix(),
                                      Material::s_WhiteMaterial);
      }

      // Apply propagation shader
      // ComputeShader::s_PropagationShader->SetParameter("inputTexture",
      // mOutlineBuffer->GetColorAttachmentRendererID(),
      // ShaderParameterType::TEXTURE);
      // Renderer::Get()->GetRendererApi()->ApplyComputeShader(
      //  ComputeShader::s_PropagationShader, mOutlineBuffer, 0, 1);

      mOutlineBuffer->Unbind();
    }*/

    // mRendererApi->Blit(*mFramebuffer,
    //                     *mPresentationBuffer,
    //                     0,
    //                     0,
    //                     mFramebuffer->GetSpecification().Width,
    //                     mFramebuffer->GetSpecification().Height);

    mRendererApi->CustomBlit(
      *mFramebuffer, *mPresentationBuffer, 0, 0, mAgxTonemapShader);
  }

  void
  SceneViewerWindow::OnImGuiRender()
  {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(800, 500));

    ImGui::Begin(GetIdentifier().c_str(), &mWindowOpened, window_flags);

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
        if (is_selected) ImGui::SetItemDefaultFocus();
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
      ImGui::InputInt("##aspectWidth", &(mSettings.AspectRatio[0]));
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
                                         (float)mSettings.ViewportSize.x /
                                           mSettings.targetAspectRatio) /
                               2.0f);
        minRect.y += diff;
        maxRect.y -= diff;
      }
      else if (((double)mSettings.ViewportSize.x /
                (double)mSettings.ViewportSize.y) > mSettings.targetAspectRatio)
      {
        float diff = std::ceil(std::fabs((float)mSettings.ViewportSize.x -
                                         (float)mSettings.ViewportSize.y *
                                           mSettings.targetAspectRatio) /
                               2.0f);
        minRect.x += diff;
        maxRect.x -= diff;
      }
    }

    // Rendering the framebuffer
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddImage(
      GetFrameBufferForImGui(), minRect, maxRect, ImVec2(0, 1), ImVec2(1, 0));

    // Check if a mesh is clicked
    glm::vec2 mousePos = { ImGui::GetMousePos().x, ImGui::GetMousePos().y };
    glm::vec2 minRectGlm = { minRect.x, minRect.y };
    glm::vec2 maxRectGlm = { maxRect.x, maxRect.y };

    if (mInputManager->GetMouseButtonDown(MOUSE_BUTTON::LEFT) &&
        mousePos.x > minRectGlm.x && mousePos.x < maxRectGlm.x &&
        mousePos.y > minRectGlm.y && mousePos.y < maxRectGlm.y)
    {
      ProcessSceneClick(mousePos - minRectGlm, { maxRectGlm - minRectGlm });
    }

    drawList->ChannelsMerge();

    auto hoverRectMin =
      ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x,
             ImGui::GetWindowPos().y + ImGui::GetCursorPos().y);

    auto hoverRectMax =
      ImVec2(ImGui::GetWindowPos().x + ImGui::GetCursorPos().x +
               ImGui::GetContentRegionAvail().x,
             ImGui::GetWindowPos().y + ImGui::GetCursorPos().y +
               ImGui::GetContentRegionAvail().y);

    // TODO: Add cursor collision with other windows that may block

    if (!mSettings.CameraMovement &&
        ImGui::IsMouseHoveringRect(hoverRectMin, hoverRectMax) &&
        mInputManager->GetMouseButton(MOUSE_BUTTON::RIGHT))
    {
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

  ImTextureID
  SceneViewerWindow::GetFrameBufferForImGui()
  {
    return (ImTextureID)mPresentationBuffer->GetColorAttachment()
      .value()
      .get()
      .GetTextureID();
  }

  glm::ivec2
  SceneViewerWindow::CalculateDesiredResolution(glm::ivec2 availableResolution,
                                                float targetAspectRatio) const
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

    Entity              entity(mEditorSelection->GetSelectedEntities().at(0),
                  mLoadedScene->GetScene().GetRegistry());
    TransformComponent& tc = entity.GetComponent<TransformComponent>();
    glm::mat4           transform = tc.GetModelMatrix();

    ImGuizmo::Manipulate(glm::value_ptr(mCamera->GetViewMatrix()),
                         glm::value_ptr(mCamera->GetProjectionMatrix()),
                         mSettings.GizmoOperation,
                         mSettings.GizmoMode,
                         glm::value_ptr(transform));

    if (ImGuizmo::IsUsing())
    {
      tc.GetPosition() = glm::vec3(transform[3]);
      tc.GetScale() = glm::vec3(glm::length(glm::vec3(transform[0])),
                                glm::length(glm::vec3(transform[1])),
                                glm::length(glm::vec3(transform[2])));

      glm::mat3 rotationMatrix =
        glm::mat3(transform); // Remove translation from matrix
      rotationMatrix = glm::mat3(glm::normalize(rotationMatrix[0]),
                                 glm::normalize(rotationMatrix[1]),
                                 glm::normalize(rotationMatrix[2]));

      // Convert rotation matrix to Euler angles (in radians)
      glm::quat rotationQuat(rotationMatrix);
      tc.GetEulerAngles() = glm::degrees(
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

  glm::vec3
  SceneViewerWindow::CalculateSelectionCenter() const
  {
    auto center = glm::vec3(0);

    for (auto entity : mEditorSelection->GetSelectedEntities())
    {
      Entity e(entity, mLoadedScene->GetScene().GetRegistry());
      center += e.GetComponent<TransformComponent>().GetPosition();
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

    if ((mFramebuffer->GetSpecification().Width != desiredResolution.x) ||
        (mFramebuffer->GetSpecification().Height != desiredResolution.y))
    {
      mFramebuffer->Resize(desiredResolution.x, desiredResolution.y);
      mIdBuffer->Resize(desiredResolution.x, desiredResolution.y);
      // mOutlineBuffer->Resize(desiredResolution.x, desiredResolution.y);
      mPresentationBuffer->Resize(desiredResolution.x, desiredResolution.y);
      mCamera->GetProperties().AspectRatio =
        (float)desiredResolution.x / (float)desiredResolution.y;
    }

    if (mFramebuffer->GetSpecification().Samples != mSettings.Samples)
    {
      mFramebuffer->SetSamples(mSettings.Samples);
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
        moduleData["settings"].contains("samples"))
    {
      mSettings.Samples = std::min(
        moduleData["settings"]["samples"].get<int32_t>(), mSettings.MaxSamples);
    }
  }

  nlohmann::json
  SceneViewerWindow::Serialize()
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

    serializedModule["id"] = GetUuid()->toString();
    serializedModule["type"] = static_cast<int>(GetModuleType());
    serializedModule["label"] = GetModuleName();

    return serializedModule;
  }

  void
  SceneViewerWindow::ProcessSceneClick(glm::vec2 const& mousePosition,
                                       glm::vec2 const& viewportSize)
  {
    unsigned int handle =
      mIdBuffer->ReadPixel(0, mousePosition.x, mousePosition.y);

    if (handle > 0)
    {
      entt::entity entity = static_cast<entt::entity>(handle);
      mEditorSelection->SelectEntity(entity);
    }
    else
    {
      mEditorSelection->ClearEntitySelection();
    }
  }
}