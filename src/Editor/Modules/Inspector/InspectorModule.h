#pragma once

#include <imgui_internal.h>
#include <math.h>

#include "Core/Scene/Scene.h"
#include "Editor/Modules/IModule.h"

#define _USE_MATH_DEFINES
#include<cmath>

namespace Dwarf {

    /// @brief Module that renders a window, containing information of selected objects or assets.
    class InspectorModule : public IModule{
        private:
            /// @brief Pointer to the currently opened scene instance.
            Scene* scene;

            /// @brief Renders the components of an entity.
            /// @param entity Entity to render in the inspector.
            void RenderComponents(Entity entity);
        public:
            InspectorModule(IViewListener *listener, Scene* scene, int id);

            /// @brief Renders the module window.
            void RenderModuleWindow() override;

            /// @brief Executes all pre frame tasks.
            void StartFrame() override {}
    };
}