#pragma once

#include "../../../utilities/dpch.h"
#include"../../scene/Scene.h"
#include<imgui_internal.h>
#include<math.h>
#include "../IModule.h"

#define _USE_MATH_DEFINES
#include<cmath>

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