#pragma once

#include "../../../utilities/dpch.h"
#include "../../asset database/AssetDatabase.h"
#include "../../asset database/AssetComponents.h"

#include<imgui_internal.h>

#include "../IModule.h"

/// @brief GUI Module to display a window for the asset directory structure.
class DebugModule : public IModule{
    private:
        AssetDatabase* assetDatabase;

    public:
        DebugModule(IViewListener *listener, AssetDatabase* assetDatabase, int id);

        /// @brief Renders the module window.
        void RenderModuleWindow() override;

        /// @brief Executes all pre frame tasks.
        void StartFrame() override {}
};