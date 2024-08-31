#pragma once
#include "Core/Asset/Creation/Material/IMaterialCreator.h"
#include "Core/Base.h"
#include "Editor/EditorView/IEditorView.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Core/Scene/IO/ISceneIO.h"
#include "Editor/Modules/IGuiModule.h"
#include "Project/IProjectSettings.h"
#include "pch.h"
#include "Window/IWindow.h"
#include "Editor/Modules/IGuiModuleFactory.h"
// #include "Editor/IEditorModel.h"

namespace Dwarf
{
  /// @brief View part of the editor's MVC structure.
  class EditorView : public IEditorView
  {
  private:
    // std::unique_ptr<IEditorModel>& m_Model;
    GraphicsApi                        m_GraphicsApi;
    std::shared_ptr<IWindow>           m_Window;
    std::shared_ptr<IGuiModuleFactory> m_GuiModuleFactory;
    std::shared_ptr<IEditor>           m_Editor;
    std::shared_ptr<ISceneIO>          m_SceneIO;
    std::shared_ptr<IAssetDatabase>    m_AssetDatabase;
    std::shared_ptr<IMaterialCreator>  m_MaterialCreator;
    std::shared_ptr<IProjectSettings>  m_ProjectSettings;

    /// @brief ID counter for GUI modules.
    int m_GuiModuleIDCount = 0;

    /// @brief List of GUI modules.
    std::vector<std::shared_ptr<IGuiModule>> m_GuiModules;

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
    RemoveWindow(std::shared_ptr<UUID> uuid);

    void
    UpdateWindowTitle() const;

  public:
    explicit EditorView(GraphicsApi                        graphicsApi,
                        std::optional<nlohmann::json>      serializedView,
                        std::shared_ptr<IEditor>           editor,
                        std::shared_ptr<IWindow>           window,
                        std::shared_ptr<IGuiModuleFactory> guiModuleFactory,
                        std::shared_ptr<ISceneIO>          sceneIO,
                        std::shared_ptr<IAssetDatabase>    assetDatabase,
                        std::shared_ptr<IMaterialCreator>  materialCreator,
                        std::shared_ptr<IProjectSettings>  projectSettings);

    /// @brief Initializes the view.
    // void
    // Init();

    /// @brief Executes all pre frame tasks.
    void
    OnUpdate() override;

    /// @brief Renders the GUI of the editor.
    void
    OnImGuiRender() override;

    void
    DockWindowToFocused() override;

    nlohmann::json
    Serialize() const override;
  };
}