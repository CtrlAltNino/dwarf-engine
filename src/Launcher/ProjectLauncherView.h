#pragma once

#ifdef __APPLE__
    #include <Metal/Metal.hpp>
    #include "Window/WindowManagerMetal.h"
#endif

#include "Utilities/Common.h"
#include "Window/IWindowManager.h"
#include "ProjectLauncherUtilities.h"
#include "ProjectLauncherModel.h"
#include "Utilities/TimeUtilities.h"
#include "Utilities/BrowserLinkOpener.h"
#include "Core/Rendering/ITexture.h"
#include "Core/Asset/TextureFileHandler.h"

namespace Dwarf {

    class ProjectLauncherView {
        private:
            IWindowManager* windowManager;

            ProjectLauncherModel* model;

            /// @brief Font loaded into IMGUI for header text
            ImFont* headerFont;

            /// @brief Font loaded into IMGUI for regular text
            ImFont* textFont;

            /// @brief Loaded image for the github icon
            ITexture* githubIcon;

            /// @brief Loaded image for the patreon icon
            ITexture* patreonIcon;

            /// @brief Loaded image for the twitter icon
            ITexture* twitterIcon;
            void RenderProjectList(int fWidth, int fHeight);

            void RenderButtons(int fWidth, int fHeight);

            void RenderFooter(int fWidth, int fHeight);

            void RenderChangeGraphicsApiModal();

            void RenderProjectNotFoundModal();

            void RenderCreateNewProjectModal();
        public:
            ProjectLauncherView();
            void Init(IWindowManager* windowManager, ProjectLauncherModel* model);
            void Render();
    };
}