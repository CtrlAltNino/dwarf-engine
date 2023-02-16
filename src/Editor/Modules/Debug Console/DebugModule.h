#pragma once

#include <imgui_internal.h>

#include "Core/Asset/AssetDatabase.h"
#include "Core/Asset/AssetComponents.h"
#include "Editor/Modules/IModule.h"

namespace Dwarf {

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
    }