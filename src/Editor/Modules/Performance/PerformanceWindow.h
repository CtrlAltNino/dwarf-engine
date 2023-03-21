#pragma once

#include "Core/Base.h"
#include "Editor/EditorModel.h"
#include "Editor/Modules/GuiModule.h"

namespace Dwarf {

    /// @brief Module to render a window that displays performance statistics.
    class PerformanceWindow : public GuiModule{
        private:
            /// @brief Pointer to the frame time.
            Ref<double> m_Frametime;

            /// @brief Pointer to the render time.
            Ref<double> m_RenderTime;
        public:
            PerformanceWindow(Ref<EditorModel> model, int id);

            /// @brief Renders the module window.
            void OnImGuiRender() override;

            void OnUpdate(double deltaTime) override;
    };
}