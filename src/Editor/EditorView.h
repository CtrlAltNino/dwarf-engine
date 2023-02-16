#pragma once

#include <imgui_internal.h>

#include "Editor/Modules/IModule.h"
#include "Editor/EditorCommon.h"

namespace Dwarf {

    /// @brief View part of the editor's MVC structure.
    class EditorView {
        private:
            /// @brief IO instance of IMGUI.
            ImGuiIO* io;

            /// @brief Interfaced controller to communicate with the controller.
            IViewListener* viewListener;

            /// @brief TODO IMGUI example function to render the base docking layout.
            void RenderDockSpace();
        public:
            EditorView(IViewListener* listener);

            /// @brief Initializes the view.
            void Init();

            /// @brief Executes all pre frame tasks.
            void StartFrame();

            /// @brief Renders the GUI of the editor.
            void RenderGui();

            /// @brief Executes all post frame tasks.
            void EndFrame();

            void DockWindowToFocused(std::string windowName);
    };
}