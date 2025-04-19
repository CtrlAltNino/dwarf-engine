#pragma once

#include "Core/Asset/Creation/Material/IMaterialCreator.hpp"
#include "Core/Asset/Creation/Shader/IShaderCreator.hpp"
#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Base.hpp"
#include "Core/Scene/IO/ISceneIO.hpp"
#include "Core/Scene/ISceneFactory.hpp"
#include "Editor/EditorView/IEditorView.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "Editor/Modules/IGuiModule.hpp"
#include "Editor/Modules/IGuiModuleFactory.hpp"
#include "Editor/Stats/IEditorStats.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "Project/IProjectSettings.hpp"
#include "Window/IWindow.hpp"
#include <condition_variable>

namespace Dwarf
{
  class EditorView : public IEditorView
  {
  private:
    GraphicsApi                        mGraphicsApi;
    std::shared_ptr<IDwarfLogger>      mLogger;
    std::shared_ptr<IWindow>           mWindow;
    std::shared_ptr<IGuiModuleFactory> mGuiModuleFactory;
    std::shared_ptr<ISceneIO>          mSceneIO;
    std::shared_ptr<ISceneFactory>     mSceneFactory;
    std::shared_ptr<IAssetDatabase>    mAssetDatabase;
    std::shared_ptr<IMaterialCreator>  mMaterialCreator;
    std::shared_ptr<IShaderCreator>    mShaderCreator;
    std::shared_ptr<IProjectSettings>  mProjectSettings;
    std::shared_ptr<ILoadedScene>      mLoadedScene;
    std::shared_ptr<IEditorStats>      mEditorStats;
    // Thread for saving the view
    std::thread             mViewSaveThread;
    std::atomic<bool>       mRunViewSaveThread = true;
    std::condition_variable mThreadCondition;
    std::mutex              mThreadMutex;

    /// @brief ID counter for GUI modules.
    int mGuiModuleIDCount = 0;

    /// @brief List of GUI modules.
    std::vector<std::unique_ptr<IGuiModule>> mGuiModules;

    /// @brief IMGUI example function to render the base docking layout.
    void
    RenderDockSpace();

    /// @brief Adds a new IMGUI window for a GUI module.
    /// @param moduleType Type of module to add a window for.
    void
    AddWindow(MODULE_TYPE moduleType);

    /// @brief Removes a IMGUI window that corresponds to a given index.
    /// @param index Index of the window to remove.
    void
    RemoveWindow(const std::shared_ptr<UUID>& uuid);

  public:
    explicit EditorView(GraphicsApi                        graphicsApi,
                        std::shared_ptr<IDwarfLogger>      logger,
                        std::shared_ptr<IProjectSettings>  projectSettings,
                        std::shared_ptr<ILoadedScene>      loadedScene,
                        std::shared_ptr<IWindow>           window,
                        std::shared_ptr<IGuiModuleFactory> guiModuleFactory,
                        std::shared_ptr<ISceneIO>          sceneIO,
                        std::shared_ptr<ISceneFactory>     sceneFactory,
                        std::shared_ptr<IAssetDatabase>    assetDatabase,
                        std::shared_ptr<IMaterialCreator>  materialCreator,
                        std::shared_ptr<IShaderCreator>    shaderCreator,
                        std::shared_ptr<IEditorStats>      editorStats);

    ~EditorView() override;

    /// @brief Executes all pre frame tasks.
    void
    OnUpdate() override;

    /// @brief Renders the GUI of the editor.
    void
    OnImGuiRender() override;

    static void
    DockWindowToFocused();

    auto
    Serialize() -> nlohmann::json override;

    void
    Shutdown() override;
  };
}