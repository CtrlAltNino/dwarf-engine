#include "Editor/Modules/SceneViewer/SceneViewerWindow.h"
#include "Core/Rendering/RendererApi/IRendererApiFactory.h"
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
    std::shared_ptr<IRendererApiFactory>       rendererApiFactory)
    : IGuiModule(ModuleLabel("Scene Viewer"),
                 ModuleType(MODULE_TYPE::SCENE_VIEWER),
                 ModuleID(std::make_shared<UUID>()))
    , m_CameraFactory(cameraFactory)
    , m_FramebufferFactory(framebufferFactory)
    , m_EditorStats(editorStats)
    , m_InputManager(inputManager)
    , m_LoadedScene(loadedScene)
    , m_EditorSelection(editorSelection)
    , m_RenderingPipelineFactory(renderingPipelineFactory)
    , m_RendererApiFactory(rendererApiFactory)
  {
    m_RendererApi = m_RendererApiFactory->Create();

    // Create rendering pipeline
    m_RenderingPipeline =
      m_RenderingPipelineFactory->Create(PipelineType::Forward);

    // Setup rendering framebuffer according to the pipeline specification
    m_Framebuffer =
      m_FramebufferFactory->Create(m_RenderingPipeline->GetSpecification());

    // Setup framebuffer for presentation
    FramebufferSpecification presentationSpec;
    presentationSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RGBA8 }
    };

    m_PresentationBuffer = m_FramebufferFactory->Create(presentationSpec);

    // Setup frame buffer for rendering ids
    FramebufferSpecification idSpec;
    idSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RED_INTEGER },
      FramebufferTextureSpecification{ FramebufferTextureFormat::DEPTH }
    };
    idSpec.Width = 512;
    idSpec.Height = 512;
    m_IdBuffer = std::move(m_FramebufferFactory->Create(idSpec));

    // Setup outline buffer

    // Setup camera
    m_Camera = m_CameraFactory->Create();
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
    std::shared_ptr<IRendererApiFactory>       rendererApiFactory)
    : IGuiModule(ModuleLabel("Scene Viewer"),
                 ModuleType(MODULE_TYPE::SCENE_VIEWER),
                 ModuleID(std::make_shared<UUID>(
                   serializedModule.t["id"].get<std::string>())))
    , m_CameraFactory(cameraFactory)
    , m_FramebufferFactory(framebufferFactory)
    , m_EditorStats(editorStats)
    , m_InputManager(inputManager)
    , m_LoadedScene(loadedScene)
    , m_EditorSelection(editorSelection)
    , m_RenderingPipelineFactory(renderingPipelineFactory)
    , m_RendererApiFactory(rendererApiFactory)
  {
    m_RendererApi = m_RendererApiFactory->Create();

    // Create rendering pipeline
    m_RenderingPipeline =
      m_RenderingPipelineFactory->Create(PipelineType::Forward);

    // Setup rendering framebuffer according to the pipeline specification
    m_Framebuffer =
      m_FramebufferFactory->Create(m_RenderingPipeline->GetSpecification());

    // Setup framebuffer for presentation
    FramebufferSpecification presentationSpec;
    presentationSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RGBA8 }
    };

    m_PresentationBuffer = m_FramebufferFactory->Create(presentationSpec);

    // Setup frame buffer for rendering ids
    FramebufferSpecification idSpec;
    idSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RED_INTEGER },
      FramebufferTextureSpecification{ FramebufferTextureFormat::DEPTH }
    };
    idSpec.Width = 512;
    idSpec.Height = 512;
    m_IdBuffer = std::move(m_FramebufferFactory->Create(idSpec));

    // Setup outline buffer

    // Setup camera
    m_Camera = m_CameraFactory->Create();
  }

  void
  SceneViewerWindow::OnUpdate()
  {
    if (m_Settings.CameraMovement &&
        m_InputManager->GetMouseButton(MOUSE_BUTTON::RIGHT))
    {
      m_Camera->OnUpdate(m_EditorStats->GetDeltaTime());
    }

    // Render scene to the framebuffer with the camera
    m_Framebuffer->Bind();
    m_RenderingPipeline->RenderScene(
      m_LoadedScene->GetScene(),
      *m_Camera,
      { m_Framebuffer->GetSpecification().Width,
        m_Framebuffer->GetSpecification().Height },
      m_Settings.RenderGrid);
    m_Framebuffer->Unbind();

    m_IdBuffer->Bind();
    m_RenderingPipeline->RenderIds(m_LoadedScene->GetScene(),
                                   *m_Camera,
                                   { m_IdBuffer->GetSpecification().Width,
                                     m_IdBuffer->GetSpecification().Height });
    m_IdBuffer->Unbind();

    /*if (m_Model->GetSelection().GetSelectedEntities().size() > 0)
    {
      m_OutlineBuffer->Clear(glm::vec4(0));
      m_OutlineBuffer->Bind();
      for (auto entity : m_Model->GetSelection().GetSelectedEntities())
      {
        Renderer::Get()->RenderEntity(entity,
                                      m_Camera->GetViewMatrix(),
                                      m_Camera->GetProjectionMatrix(),
                                      Material::s_WhiteMaterial);
      }

      // Apply propagation shader
      // ComputeShader::s_PropagationShader->SetParameter("inputTexture",
      // m_OutlineBuffer->GetColorAttachmentRendererID(),
      // ShaderParameterType::TEXTURE);
      // Renderer::Get()->GetRendererApi()->ApplyComputeShader(
      //  ComputeShader::s_PropagationShader, m_OutlineBuffer, 0, 1);

      m_OutlineBuffer->Unbind();
    }*/

    m_RendererApi->Blit(*m_Framebuffer,
                        *m_PresentationBuffer,
                        0,
                        0,
                        m_Framebuffer->GetSpecification().Width,
                        m_Framebuffer->GetSpecification().Height);
  }

  void
  SceneViewerWindow::OnImGuiRender()
  {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(800, 500));

    ImGui::Begin(GetIdentifier().c_str(), &m_WindowOpened, window_flags);

    ImGui::PopStyleVar(2);
    static std::array<std::string, 3> renderingModes = { "Free",
                                                         "Aspect Ratio",
                                                         "Fixed Resolution" };

    ImGui::PushItemWidth(150);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    // Rendering mode combo
    if (ImGui::BeginCombo(
          "##renderingMode",
          renderingModes[(int)m_Settings.RenderingConstraint].c_str()))
    {
      for (int n = 0; n < 3; n++)
      {
        const bool is_selected = ((int)m_Settings.RenderingConstraint == n);

        // ==================== Graphics Selectable ====================
        if (ImGui::Selectable(
              renderingModes[n].c_str(), is_selected, 0, ImVec2(0, 16 + 10)))
        {
          m_Settings.RenderingConstraint = (RENDERING_CONSTRAINT)n;
        }

        // Set the initial focus when opening the combo (scrolling + keyboard
        // navigation focus)
        if (is_selected) ImGui::SetItemDefaultFocus();
      }

      ImGui::EndCombo();
    }

    ImGui::SameLine(0, 5);

    // Guizmo model toggle
    if (m_Settings.GizmoMode == ImGuizmo::MODE::LOCAL)
    {
      if (ImGui::Button("Local"))
      {
        m_Settings.GizmoMode = ImGuizmo::MODE::WORLD;
      }
    }
    else
    {
      if (ImGui::Button("World"))
      {
        m_Settings.GizmoMode = ImGuizmo::MODE::LOCAL;
      }
    }

    ImGui::SameLine(0, 5);

    // Guizmo type translate button
    if (ImGui::Button("Translate"))
    {
      m_Settings.GizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
    }

    ImGui::SameLine(0, 5);

    // Guizmo type rotate button
    if (ImGui::Button("Rotate"))
    {
      m_Settings.GizmoOperation = ImGuizmo::OPERATION::ROTATE;
    }

    ImGui::SameLine(0, 5);

    // Guizmo type scale button
    if (ImGui::Button("Scale"))
    {
      m_Settings.GizmoOperation = ImGuizmo::OPERATION::SCALE;
    }

    // Rendering mode specific settings input

    if (m_Settings.RenderingConstraint == RENDERING_CONSTRAINT::ASPECT_RATIO)
    {
      ImGui::SameLine(0, 10);
      ImGui::Text("Aspect ratio");

      ImGui::SameLine(0, 10);
      ImGui::InputInt("##aspectWidth", &(m_Settings.AspectRatio[0]));
      ImGui::SameLine(0, 10);
      ImGui::InputInt("##aspectHeight", &(m_Settings.AspectRatio[1]));

      m_Settings.AspectRatio[0] =
        std::max(1, std::min(MAX_RESOLUTION_WIDTH, m_Settings.AspectRatio[0]));
      m_Settings.AspectRatio[1] =
        std::max(1, std::min(MAX_RESOLUTION_HEIGHT, m_Settings.AspectRatio[1]));
    }
    else if (m_Settings.RenderingConstraint ==
             RENDERING_CONSTRAINT::FIXED_RESOLUTION)
    {
      ImGui::SameLine(0, 10);
      ImGui::Text("Resolution");
      ImGui::SameLine(0, 10);
      ImGui::InputInt2("", m_Settings.Resolution.data());
      m_Settings.Resolution[0] =
        std::max(MIN_RESOLUTION_WIDTH,
                 std::min(MAX_RESOLUTION_WIDTH, m_Settings.Resolution[0]));
      m_Settings.Resolution[1] =
        std::max(MIN_RESOLUTION_WIDTH,
                 std::min(MAX_RESOLUTION_WIDTH, m_Settings.Resolution[1]));
    }

    ImGui::SameLine(0, 5);

    ImGui::Checkbox("Render Grid", &m_Settings.RenderGrid);

    ImGui::PopStyleVar();
    ImGui::PopItemWidth();

    // Calculating the rect size for the framebuffer
    ImVec2 minRect = ImGui::GetCursorScreenPos();
    ImVec2 maxRect(
      ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
      ImGui::GetCursorScreenPos().y + ImGui::GetContentRegionAvail().y);

    m_Settings.ViewportSize = glm::ivec2(ImGui::GetContentRegionAvail().x,
                                         ImGui::GetContentRegionAvail().y);

    UpdateFramebuffer();

    if (m_Settings.RenderingConstraint != RENDERING_CONSTRAINT::FREE)
    {
      if (((float)m_Settings.ViewportSize.x /
           (float)m_Settings.ViewportSize.y) < m_Settings.targetAspectRatio)
      {
        float diff = std::ceil(std::fabs((float)m_Settings.ViewportSize.y -
                                         (float)m_Settings.ViewportSize.x /
                                           m_Settings.targetAspectRatio) /
                               2.0f);
        minRect.y += diff;
        maxRect.y -= diff;
      }
      else if (((double)m_Settings.ViewportSize.x /
                (double)m_Settings.ViewportSize.y) >
               m_Settings.targetAspectRatio)
      {
        float diff = std::ceil(std::fabs((float)m_Settings.ViewportSize.x -
                                         (float)m_Settings.ViewportSize.y *
                                           m_Settings.targetAspectRatio) /
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

    if (m_InputManager->GetMouseButtonDown(MOUSE_BUTTON::LEFT) &&
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

    if (!m_Settings.CameraMovement &&
        ImGui::IsMouseHoveringRect(hoverRectMin, hoverRectMax) &&
        m_InputManager->GetMouseButton(MOUSE_BUTTON::RIGHT))
    {
      m_Settings.CameraMovement = true;
      ImGui::FocusWindow(ImGui::GetCurrentWindow());
    }
    else if (m_Settings.CameraMovement &&
             !m_InputManager->GetMouseButton(MOUSE_BUTTON::RIGHT))
    {
      m_Settings.CameraMovement = false;
    }

    if (m_EditorSelection->GetSelectedEntities().size() == 1)
    {
      RenderGizmos(minRect, maxRect);
    }
    ImGui::End();
  }

  ImTextureID
  SceneViewerWindow::GetFrameBufferForImGui()
  {
    return (ImTextureID)m_PresentationBuffer->GetColorAttachment()
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

    TransformComponent& tc = m_EditorSelection->GetSelectedEntities()
                               .at(0)
                               .GetComponent<TransformComponent>();
    glm::mat4 transform = tc.GetModelMatrix();

    ImGuizmo::Manipulate(glm::value_ptr(m_Camera->GetViewMatrix()),
                         glm::value_ptr(m_Camera->GetProjectionMatrix()),
                         m_Settings.GizmoOperation,
                         m_Settings.GizmoMode,
                         glm::value_ptr(transform));

    if (ImGuizmo::IsUsing())
    {
      tc.GetPosition() = glm::vec3(transform[3]);
      glm::vec3 rotation;

      ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform),
                                            glm::value_ptr(tc.GetPosition()),
                                            glm::value_ptr(rotation),
                                            glm::value_ptr(tc.GetScale()));

      glm::vec3 deltaRotation = rotation - tc.GetEulerAngles();

      tc.GetEulerAngles() += deltaRotation;
    }
  }

  void
  SceneViewerWindow::UpdateGizmoType()
  {
    if (ImGui::IsWindowFocused() &&
        !m_InputManager->GetMouseButton(MOUSE_BUTTON::RIGHT))
    {
      using enum KEYCODE;
      if (m_InputManager->GetKeyDown(W))
      {
        m_Settings.GizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
      }
      else if (m_InputManager->GetKeyDown(E))
      {
        m_Settings.GizmoOperation = ImGuizmo::OPERATION::ROTATE;
      }
      else if (m_InputManager->GetKeyDown(R))
      {
        m_Settings.GizmoOperation = ImGuizmo::OPERATION::SCALE;
      }
    }
  }

  glm::vec3
  SceneViewerWindow::CalculateSelectionCenter() const
  {
    auto center = glm::vec3(0);

    for (auto entity : m_EditorSelection->GetSelectedEntities())
    {
      center += entity.GetComponent<TransformComponent>().GetPosition();
    }

    return center / (float)m_EditorSelection->GetSelectedEntities().size();
  }

  void
  SceneViewerWindow::UpdateFramebuffer()
  {
    glm::ivec2 desiredResolution;

    switch (m_Settings.RenderingConstraint)
    {
      case RENDERING_CONSTRAINT::FREE:
        {
          desiredResolution = m_Settings.ViewportSize;
          m_Settings.targetAspectRatio =
            (float)desiredResolution.x / (float)desiredResolution.y;
        }
        break;
      case RENDERING_CONSTRAINT::FIXED_RESOLUTION:
        {
          desiredResolution =
            glm::ivec2(m_Settings.Resolution[0], m_Settings.Resolution[1]);
          m_Settings.targetAspectRatio =
            (float)m_Settings.Resolution[0] / (float)m_Settings.Resolution[1];
        }
        break;
      case RENDERING_CONSTRAINT::ASPECT_RATIO:
        {
          desiredResolution =
            CalculateDesiredResolution(m_Settings.ViewportSize,
                                       (float)m_Settings.AspectRatio[0] /
                                         (float)m_Settings.AspectRatio[1]);
          m_Settings.targetAspectRatio =
            (float)desiredResolution.x / (float)desiredResolution.y;
        }
        break;
    }

    if ((m_Framebuffer->GetSpecification().Width != desiredResolution.x) ||
        (m_Framebuffer->GetSpecification().Height != desiredResolution.y))
    {
      m_Framebuffer->Resize(desiredResolution.x, desiredResolution.y);
      m_IdBuffer->Resize(desiredResolution.x, desiredResolution.y);
      // m_OutlineBuffer->Resize(desiredResolution.x, desiredResolution.y);
      m_PresentationBuffer->Resize(desiredResolution.x, desiredResolution.y);
      m_Camera->GetProperties().AspectRatio =
        (float)desiredResolution.x / (float)desiredResolution.y;
    }
  }

  void
  SceneViewerWindow::Deserialize(nlohmann::json moduleData)
  {

    m_Camera->GetProperties().Transform.GetPosition() = {
      moduleData["camera"]["position"]["x"],
      moduleData["camera"]["position"]["y"],
      moduleData["camera"]["position"]["z"]
    };

    m_Camera->GetProperties().Transform.GetEulerAngles() = {
      moduleData["camera"]["rotation"]["x"],
      moduleData["camera"]["rotation"]["y"],
      moduleData["camera"]["rotation"]["z"]
    };

    m_Camera->GetProperties().Fov = moduleData["camera"]["fov"];

    m_Camera->GetProperties().NearPlane = moduleData["camera"]["near"];
    m_Camera->GetProperties().FarPlane = moduleData["camera"]["far"];

    m_Settings.AspectRatio[0] =
      moduleData["settings"]["aspectRatioConstraint"]["x"];
    m_Settings.AspectRatio[1] =
      moduleData["settings"]["aspectRatioConstraint"]["y"];

    m_Settings.Resolution[0] =
      moduleData["settings"]["resolutionConstraint"]["x"];
    m_Settings.Resolution[1] =
      moduleData["settings"]["resolutionConstraint"]["y"];

    m_Settings.RenderingConstraint =
      (RENDERING_CONSTRAINT)moduleData["settings"]["constraintType"];

    m_Settings.RenderGrid = moduleData["settings"]["renderGrid"];
  }

  nlohmann::json
  SceneViewerWindow::Serialize() const
  {
    nlohmann::json serializedModule;

    serializedModule["camera"]["position"]["x"] =
      m_Camera->GetProperties().Transform.GetPosition().x;
    serializedModule["camera"]["position"]["y"] =
      m_Camera->GetProperties().Transform.GetPosition().y;
    serializedModule["camera"]["position"]["z"] =
      m_Camera->GetProperties().Transform.GetPosition().z;

    serializedModule["camera"]["rotation"]["x"] =
      m_Camera->GetProperties().Transform.GetEulerAngles().x;
    serializedModule["camera"]["rotation"]["y"] =
      m_Camera->GetProperties().Transform.GetEulerAngles().y;
    serializedModule["camera"]["rotation"]["z"] =
      m_Camera->GetProperties().Transform.GetEulerAngles().z;

    serializedModule["camera"]["fov"] = m_Camera->GetProperties().Fov;
    serializedModule["camera"]["near"] = m_Camera->GetProperties().NearPlane;
    serializedModule["camera"]["far"] = m_Camera->GetProperties().FarPlane;

    serializedModule["settings"]["aspectRatioConstraint"]["x"] =
      m_Settings.AspectRatio[0];
    serializedModule["settings"]["aspectRatioConstraint"]["y"] =
      m_Settings.AspectRatio[1];

    serializedModule["settings"]["resolutionConstraint"]["x"] =
      m_Settings.Resolution[0];
    serializedModule["settings"]["resolutionConstraint"]["y"] =
      m_Settings.Resolution[1];

    serializedModule["settings"]["constraintType"] =
      m_Settings.RenderingConstraint;

    serializedModule["settings"]["renderGrid"] = m_Settings.RenderGrid;

    serializedModule["id"] = GetUuid()->ToString();
    serializedModule["type"] = static_cast<int>(GetModuleType());
    serializedModule["label"] = GetModuleName();

    return serializedModule;
  }

  void
  SceneViewerWindow::ProcessSceneClick(glm::vec2 const& mousePosition,
                                       glm::vec2 const& viewportSize)
  {
    unsigned int handle =
      m_IdBuffer->ReadPixel(0, mousePosition.x, mousePosition.y);

    if (handle > 0)
    {
      entt::entity entity = static_cast<entt::entity>(handle);
      m_EditorSelection->SelectEntity(
        Entity(entity, m_LoadedScene->GetScene().GetRegistry()));
    }
    else
    {
      m_EditorSelection->ClearEntitySelection();
    }
  }
}