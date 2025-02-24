#include "Core/Asset/Creation/Material/IMaterialCreator.h"
#include "Core/Base.h"
#include "Core/Scene/ISceneFactory.h"
#include "Editor/Modules/IGuiModuleFactory.h"
#include "pch.h"
#include <memory>
#include <nfd.h>
#include <nlohmann/json_fwd.hpp>

#include "EditorView.h"

#include "Core/Asset/Database/AssetDatabase.h"
#include "UI/DwarfUI.h"

namespace Dwarf
{
  EditorView::EditorView(GraphicsApi                        graphicsApi,
                         std::shared_ptr<IDwarfLogger>      logger,
                         std::shared_ptr<IProjectSettings>  projectSettings,
                         std::shared_ptr<ILoadedScene>      loadedScene,
                         std::shared_ptr<IWindow>           window,
                         std::shared_ptr<IGuiModuleFactory> guiModuleFactory,
                         std::shared_ptr<ISceneIO>          sceneIO,
                         std::shared_ptr<ISceneFactory>     sceneFactory,
                         std::shared_ptr<IAssetDatabase>    assetDatabase,
                         std::shared_ptr<IMaterialCreator>  materialCreator,
                         std::shared_ptr<IEditorStats>      editorStats)
    : m_GraphicsApi(graphicsApi)
    , m_Logger(logger)
    , m_ProjectSettings(projectSettings)
    , m_LoadedScene(loadedScene)
    , m_Window(window)
    , m_GuiModuleFactory(guiModuleFactory)
    , m_SceneIO(sceneIO)
    , m_SceneFactory(sceneFactory)
    , m_AssetDatabase(assetDatabase)
    , m_MaterialCreator(materialCreator)
    , m_EditorStats(editorStats)
  {
    m_Logger->LogDebug(Log("Creating EditorView", "EditorView"));

    nlohmann::json serializedView = m_ProjectSettings->GetSerializedView();
    m_Logger->LogDebug(
      Log(fmt::format("Deserializing EditorView:\n{}", serializedView.dump(2)),
          "EditorView"));

    if (serializedView.contains("modules"))
    {
      m_Logger->LogDebug(Log("Module key found", "EditorView"));
      for (auto module : serializedView.at("modules"))
      {
        m_Logger->LogDebug(
          Log(fmt::format("Deserializing module:\n{}", module.dump(2)),
              "EditorView"));
        m_GuiModules.push_back(std::move(
          m_GuiModuleFactory->CreateGuiModule(SerializedModule(module))));
      }
    }

    // Start a thread that saves the view every 5 seconds
    m_ViewSaveThread = std::thread(
      [this]()
      {
        while (m_RunViewSaveThread.load())
        {
          {
            std::unique_lock<std::mutex> lock(m_ThreadMutex);
            m_ThreadCondition.wait_for(lock,
                                       std::chrono::seconds(5),
                                       [this]
                                       { return m_RunViewSaveThread.load(); });
          }
          m_ProjectSettings->UpdateSerializedView(Serialize());
          m_ProjectSettings->Save();
        }
      });
  }

  EditorView::~EditorView()
  {
    m_Logger->LogDebug(Log("Destroying EditorView", "EditorView"));

    {
      std::lock_guard<std::mutex> lock(m_ThreadMutex);
      m_RunViewSaveThread.store(false);
    }
    m_ThreadCondition.notify_one();
    m_ViewSaveThread.join();

    m_Logger->LogDebug(Log("EditorView destroyed", "EditorView"));
  }

  void
  EditorView::RenderDockSpace()
  {
    // If you strip some features of, this demo is pretty much equivalent to
    // calling DockSpaceOverViewport()! In most cases you should be able to just
    // call DockSpaceOverViewport() and ignore all the code below! In this
    // specific demo, we are not using DockSpaceOverViewport() because:
    // - we allow the host window to be floating/moveable instead of filling the
    // viewport (when opt_fullscreen == false)
    // - we allow the host window to have padding (when opt_padding == true)
    // - we have a local menu bar in the host window (vs. you could use
    // BeginMainMenuBar() + DockSpaceOverViewport() in your code!) TL;DR; this
    // demo is more complicated than what you would normally use. If we removed
    // all the options we are showcasing, this demo would become:
    //     void ShowExampleAppDockSpace()
    //     {
    //         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    //     }
    static bool               p_open = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent
    // window not dockable into, because it would be confusing to have two
    // docking targets within each others.
    ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |=
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will
    // render our background and handle the pass-thru hole, so we ask Begin() to
    // not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
      window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window
    // is collapsed). This is because we want to keep our DockSpace() active. If
    // a DockSpace() is inactive, all active windows docked into it will lose
    // their parent and become undocked. We cannot preserve the docking
    // relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in
    // limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::PushStyleColor(ImGuiCol_MenuBarBg,
                          ImVec4(23 / 255.0f, 26 / 255.0f, 32 / 255.0f, 1));
    ImGui::Begin("DockSpace", &p_open, window_flags);
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor();

    // Submit the DockSpace
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("Create new scene"))
        {
          // TODO: check for unsaved changes in scene
          std::unique_ptr<IScene> newScene =
            m_SceneFactory->CreateDefaultScene();
          m_LoadedScene->SetScene(std::move(newScene));
        }
        if (ImGui::MenuItem("Save scene"))
        {
          // if (SceneUtilities::SaveScene(m_Model->GetScene()))
          // {
          //   AssetDatabase::Import(m_Model->GetScene()->GetPath());
          //   SceneUtilities::SetLastOpenedScene(m_Model->GetScene()->GetPath());
          //   UpdateWindowTitle();
          // }

          m_SceneIO->SaveScene(m_LoadedScene->GetScene());
        }
        if (ImGui::MenuItem("Save scene as"))
        {
          // if (SceneUtilities::SaveSceneDialog(m_Model->GetScene()))
          // {
          //   AssetDatabase::Import(m_Model->GetScene()->GetPath());
          //   SceneUtilities::SetLastOpenedScene(m_Model->GetScene()->GetPath());
          //   UpdateWindowTitle();
          // }

          m_SceneIO->SaveSceneDialog(m_LoadedScene->GetScene());
        }
        if (ImGui::MenuItem("Load scene"))
        {
          std::unique_ptr<IScene> loadedScene = m_SceneIO->LoadSceneDialog();
          if (loadedScene)
          {
            m_LoadedScene->SetScene(std::move(loadedScene));
            UpdateWindowTitle();
          }
        }

        if (ImGui::MenuItem("Return to project launcher"))
        {
          m_EditorStats->SetCloseSignal(true);
          m_EditorStats->SetReturnToLauncher(true);
        }

        if (ImGui::MenuItem("Quit"))
        {
          m_EditorStats->SetCloseSignal(true);
          m_EditorStats->SetReturnToLauncher(false);
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Assets"))
      {
        if (ImGui::MenuItem("Create new material"))
        {
          m_MaterialCreator->CreateMaterialAsset();
        }
        ImGui::MenuItem("Import Assets");
        if (ImGui::MenuItem("Reimport Assets"))
        {
          m_AssetDatabase->ReimportAll();
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Scene"))
      {
        if (ImGui::MenuItem("Create empty object"))
        {
          Entity newEntity =
            m_LoadedScene->GetScene().CreateEntity("New object");
          newEntity.AddComponent<MeshRendererComponent>();
        }
        if (ImGui::MenuItem("Create light"))
        {
          Entity newEntity =
            m_LoadedScene->GetScene().CreateEntity("New Light");
          newEntity.AddComponent<LightComponent>();
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Window"))
      {
        using enum MODULE_TYPE;
        if (ImGui::MenuItem("Scene Viewer"))
        {
          AddWindow(SCENE_VIEWER);
        }
        if (ImGui::MenuItem("Scene Hierarchy"))
        {
          AddWindow(SCENE_GRAPH);
        }
        if (ImGui::MenuItem("Performance"))
        {
          AddWindow(PERFORMANCE);
        }
        if (ImGui::MenuItem("Asset Browser"))
        {
          AddWindow(ASSET_BROWSER);
        }
        if (ImGui::MenuItem("Inspector"))
        {
          AddWindow(INSPECTOR);
        }
        if (ImGui::MenuItem("Debug"))
        {
          AddWindow(DEBUG);
        }
        ImGui::MenuItem("Console");
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    ImGui::End();
  }

  void
  EditorView::OnUpdate()
  {
    for (int i = 0; i < m_GuiModules.size(); i++)
    {
      m_GuiModules.at(i)->OnUpdate();
    }
  }

  void
  EditorView::OnImGuiRender()
  {

    ImGui::PushStyleColor(ImGuiCol_Text, COL_TEXT);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, COL_BG_DIM);
    ImGui::PushStyleColor(ImGuiCol_Border, COL_DIM2);

    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, COL_WINDOW_HEADER_FOCUSED);
    ImGui::PushStyleColor(ImGuiCol_TitleBg, COL_WINDOW_HEADER_UNFOCUSED);

    ImGui::PushStyleColor(ImGuiCol_TabUnfocused, COL_TAB_UNFOCUSED);
    ImGui::PushStyleColor(ImGuiCol_TabUnfocusedActive,
                          COL_TAB_UNFOCUSED_ACTIVE);

    ImGui::PushStyleColor(ImGuiCol_Tab, COL_TAB);
    ImGui::PushStyleColor(ImGuiCol_TabActive, COL_TAB_ACTIVE);
    ImGui::PushStyleColor(ImGuiCol_TabHovered, COL_TAB_HOVERED);

    ImGui::PushStyleColor(ImGuiCol_PopupBg, COL_POPUP_BG);

    ImGui::PushStyleColor(ImGuiCol_Button, COL_BUTTON);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, COL_BUTTON_ACTIVE);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COL_BUTTON_HOVERED);

    ImGui::PushStyleColor(ImGuiCol_Header, COL_BUTTON);
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, COL_BUTTON_ACTIVE);
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, COL_BUTTON_HOVERED);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));

    // Render default stuff
    RenderDockSpace();

    // ImGui::ShowDemoWindow();

    // Render modules
    for (int i = 0; i < m_GuiModules.size(); i++)
    {
      if (m_GuiModules.at(i)->GetWindowClose())
      {
        RemoveWindow(m_GuiModules.at(i)->GetUuid());
      }
      else
      {
        m_GuiModules.at(i)->OnImGuiRender();
      }
    }

    ImGui::PopStyleColor(17);
    ImGui::PopStyleVar(4);
  }

  void
  EditorView::DockWindowToFocused()
  {
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");

    static ImGuiDockNodeFlags dockspace_flags =
      ImGuiDockNodeFlags_PassthruCentralNode;

    ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
    ImGui::DockBuilderAddNode(dockspace_id,
                              dockspace_flags | ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderFinish(dockspace_id);
  }

  void
  EditorView::AddWindow(MODULE_TYPE moduleType)
  {
    std::unique_ptr<IGuiModule> guiModule =
      m_GuiModuleFactory->CreateGuiModule(moduleType);

    if (guiModule)
    {
      m_GuiModules.push_back(std::move(guiModule));
      m_ProjectSettings->UpdateSerializedView(Serialize());
      m_ProjectSettings->Save();
    }
  }

  void
  EditorView::RemoveWindow(std::shared_ptr<UUID> uuid)
  {
    for (int i = 0; i < m_GuiModules.size(); i++)
    {
      if (*m_GuiModules[i]->GetUuid() == *uuid)
      {
        m_GuiModules.erase(m_GuiModules.begin() + i);
      }
    }
    m_ProjectSettings->UpdateSerializedView(Serialize());
    m_ProjectSettings->Save();
  }

  void
  EditorView::UpdateWindowTitle() const
  {
    std::string windowTitle = "Dwarf Engine Editor - ";
    windowTitle.append(m_ProjectSettings->GetProjectName());
    windowTitle.append(" - ");
    windowTitle.append(m_LoadedScene->GetScene().GetProperties().GetName());
    windowTitle.append(" <");
    windowTitle.append(graphicsApiNames[(int)m_GraphicsApi]);
    windowTitle.append(">");

    m_Window->SetWindowTitle(windowTitle);
  }

  nlohmann::json
  EditorView::Serialize()
  {
    nlohmann::json j;
    j["modules"] = nlohmann::json::array();
    for (int i = 0; i < m_GuiModules.size(); i++)
    {
      j["modules"].push_back(m_GuiModules.at(i)->Serialize());
    }
    return j;
  }
}