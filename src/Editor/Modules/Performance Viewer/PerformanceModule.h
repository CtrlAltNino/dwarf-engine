#pragma once

#include <imgui_internal.h>

#include "Editor/Modules/IModule.h"
#include "Window/IWindowManager.h"

namespace Dwarf {

    /// @brief Module to render a window that displays performance statistics.
    class PerformanceModule : public IModule{
        private:
            /// @brief Pointer to the frame time.
            double* frametime;

            /// @brief Pointer to the render time.
            double* renderTime;
        public:
            PerformanceModule(IViewListener *listener, double* frametime, double* renderTime, int id);

            /// @brief Renders the module window.
            void RenderModuleWindow() override;

            /// @brief Executes all pre frame tasks.
            void StartFrame() override {}
    };
}