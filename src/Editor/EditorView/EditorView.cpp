#include "EditorView.h"
#include "Core/Asset/Creation/Material/IMaterialCreator.h"
#include "Core/Base.h"
#include "Core/Scene/ISceneFactory.h"
#include "Editor/Modules/IGuiModuleFactory.h"
#include "UI/DwarfUI.h"
#include "pch.h"
#include <fmt/format.h>
#include <memory>
#include <nfd.h>
#include <nlohmann/json_fwd.hpp>

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
    : mGraphicsApi(graphicsApi)
    , mLogger(logger)
    , mProjectSettings(projectSettings)
    , mLoadedScene(loadedScene)
    , mWindow(window)
    , mGuiModuleFactory(guiModuleFactory)
    , mSceneIO(sceneIO)
    , mSceneFactory(sceneFactory)
    , mAssetDatabase(assetDatabase)
    , mMaterialCreator(materialCreator)
    , mEditorStats(editorStats)
  {
    mLogger->LogDebug(Log("Creating EditorView", "EditorView"));

    nlohmann::json serializedView = mProjectSettings->GetSerializedView();
    mLogger->LogDebug(
      Log(fmt::format("Deserializing EditorView:\n{}", serializedView.dump(2)),
          "EditorView"));

    if (serializedView.contains("windowMaximized"))
    {
      mWindow->setShowWindowMaximized(
        serializedView["windowMaximized"].get<bool>());
    }

    if (serializedView.contains("modules"))
    {
      mLogger->LogDebug(Log("Module key found", "EditorView"));
      for (auto module : serializedView.at("modules"))
      {
        mLogger->LogDebug(
          Log(fmt::format("Deserializing module:\n{}", module.dump(2)),
              "EditorView"));
        mGuiModules.push_back(std::move(
          mGuiModuleFactory->CreateGuiModule(SerializedModule(module))));
      }
    }

    // Start a thread that saves the view every 5 seconds
    mViewSaveThread = std::thread(
      [this]()
      {
        while (mRunViewSaveThread.load())
        {
          {
            std::unique_lock<std::mutex> lock(mThreadMutex);
            mThreadCondition.wait_for(lock,
                                      std::chrono::seconds(5),
                                      [this]
                                      { return !mRunViewSaveThread.load(); });
            mProjectSettings->UpdateSerializedView(Serialize());
            mProjectSettings->Save();
          }
        }
      });
  }

  EditorView::~EditorView()
  {
    mLogger->LogDebug(Log("Destroying EditorView", "EditorView"));

    if (mRunViewSaveThread)
    {
      {
        std::lock_guard<std::mutex> lock(mThreadMutex);
        mRunViewSaveThread.store(false);
      }
      mThreadCondition.notify_one();
      mViewSaveThread.join();
    }

    mLogger->LogDebug(Log("EditorView destroyed", "EditorView"));
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
            mSceneFactory->CreateDefaultScene();
          mLoadedScene->SetScene(std::move(newScene));
        }
        if (ImGui::MenuItem("Save scene"))
        {
          // if (SceneUtilities::SaveScene(mModel->GetScene()))
          // {
          //   AssetDatabase::Import(mModel->GetScene()->GetPath());
          //   SceneUtilities::SetLastOpenedScene(mModel->GetScene()->GetPath());
          //   UpdateWindowTitle();
          // }

          mSceneIO->SaveScene(mLoadedScene->GetScene());
        }
        if (ImGui::MenuItem("Save scene as"))
        {
          // if (SceneUtilities::SaveSceneDialog(mModel->GetScene()))
          // {
          //   AssetDatabase::Import(mModel->GetScene()->GetPath());
          //   SceneUtilities::SetLastOpenedScene(mModel->GetScene()->GetPath());
          //   UpdateWindowTitle();
          // }

          mSceneIO->SaveSceneDialog(mLoadedScene->GetScene());
        }
        if (ImGui::MenuItem("Load scene"))
        {
          std::unique_ptr<IScene> loadedScene = mSceneIO->LoadSceneDialog();
          if (loadedScene)
          {
            mLoadedScene->SetScene(std::move(loadedScene));
            UpdateWindowTitle();
          }
        }

        if (ImGui::MenuItem("Return to project launcher"))
        {
          mEditorStats->SetCloseSignal(true);
          mEditorStats->SetReturnToLauncher(true);
        }

        if (ImGui::MenuItem("Quit"))
        {
          mEditorStats->SetCloseSignal(true);
          mEditorStats->SetReturnToLauncher(false);
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Assets"))
      {
        if (ImGui::MenuItem("Create new material"))
        {
          mMaterialCreator->CreateMaterialAsset();
        }
        ImGui::MenuItem("Import Assets");
        if (ImGui::MenuItem("Reimport Assets"))
        {
          mAssetDatabase->ReimportAll();
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Scene"))
      {
        if (ImGui::MenuItem("Create empty object"))
        {
          Entity newEntity =
            mLoadedScene->GetScene().CreateEntity("New object");
          newEntity.AddComponent<MeshRendererComponent>();
        }
        if (ImGui::MenuItem("Create light"))
        {
          Entity newEntity = mLoadedScene->GetScene().CreateEntity("New Light");
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

    {
      std::unique_lock<std::mutex> lock(mThreadMutex);
      for (int i = 0; i < mGuiModules.size(); i++)
      {
        mGuiModules.at(i)->OnUpdate();
      }
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
    for (int i = 0; i < mGuiModules.size(); i++)
    {
      if (mGuiModules.at(i)->GetWindowClose())
      {
        RemoveWindow(mGuiModules.at(i)->GetUuid());
      }
      else
      {
        mGuiModules.at(i)->OnImGuiRender();
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
      mGuiModuleFactory->CreateGuiModule(moduleType);

    if (guiModule)
    {
      mGuiModules.push_back(std::move(guiModule));
      mProjectSettings->UpdateSerializedView(Serialize());
      mProjectSettings->Save();
    }
  }

  void
  EditorView::RemoveWindow(std::shared_ptr<UUID> uuid)
  {
    for (int i = 0; i < mGuiModules.size(); i++)
    {
      if (*mGuiModules[i]->GetUuid() == *uuid)
      {
        mGuiModules.erase(mGuiModules.begin() + i);
      }
    }
    mProjectSettings->UpdateSerializedView(Serialize());
    mProjectSettings->Save();
  }

  void
  EditorView::UpdateWindowTitle() const
  {
    std::string windowTitle = "Dwarf Engine Editor - ";
    windowTitle.append(mProjectSettings->GetProjectName());
    windowTitle.append(" - ");
    windowTitle.append(mLoadedScene->GetScene().GetProperties().GetName());
    windowTitle.append(" <");
    windowTitle.append(graphicsApiNames[(int)mGraphicsApi]);
    windowTitle.append(">");

    mWindow->setWindowTitle(windowTitle);
  }

  nlohmann::json
  EditorView::Serialize()
  {
    nlohmann::json j;
    j["windowMaximized"] = mWindow->isWindowMaximized();
    j["modules"] = nlohmann::json::array();
    for (int i = 0; i < mGuiModules.size(); i++)
    {
      j["modules"].push_back(mGuiModules.at(i)->Serialize());
    }
    return j;
  }

  void
  EditorView::Shutdown()
  {
    if (mRunViewSaveThread)
    {
      {
        std::lock_guard<std::mutex> lock(mThreadMutex);
        mRunViewSaveThread.store(false);
        mGuiModules.clear();
      }
      mThreadCondition.notify_one();
      mViewSaveThread.join();
    }
  }
}