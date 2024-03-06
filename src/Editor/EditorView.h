#pragma once

#include <imgui_internal.h>

#include "Core/Base.h"
#include "Editor/Modules/GuiModule.h"
#include "Editor/EditorModel.h"
#include "Core/UI/DwarfUI.h"

namespace Dwarf
{

    /// @brief View part of the editor's MVC structure.
    class EditorView
    {
    private:
        Ref<EditorModel> m_Model;

        /// @brief ID counter for GUI modules.
        int m_GuiModuleIDCount = 0;

        /// @brief List of GUI modules.
        std::vector<Ref<GuiModule>> m_GuiModules;

        /// @brief IMGUI example function to render the base docking layout.
        void RenderDockSpace();

        /// @brief Adds a new IMGUI window for a GUI module.
        /// @param moduleType Type of module to add a window for.
        void AddWindow(MODULE_TYPE moduleType);

        /// @brief Removes a IMGUI window that corresponds to a given index.
        /// @param index Index of the window to remove.
        void RemoveWindow(int index);

    public:
        explicit EditorView(Ref<EditorModel> model);

        /// @brief Initializes the view.
        void Init();

        /// @brief Executes all pre frame tasks.
        void OnUpdate(double deltaTime);

        /// @brief Renders the GUI of the editor.
        void OnImGuiRender();

        void DockWindowToFocused();
    };
}