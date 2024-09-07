#pragma once

#include "IProjectLauncherData.h"
#include "Launcher/IProjectLauncher.h"
#include "Logging/IDwarfLogger.h"
#include "Project/ProjectTypes.h"
#include "Launcher/View/IProjectLauncherView.h"
#include "Window/IWindow.h"
#include "pch.h"

namespace Dwarf
{
  class ProjectLauncher : public IProjectLauncher
  {
  private:
    std::shared_ptr<IWindow>              m_Window;
    std::shared_ptr<IProjectLauncherView> m_View;
    std::shared_ptr<IProjectLauncherData> m_Data;
    std::shared_ptr<IDwarfLogger>         m_Logger;

  public:
    ProjectLauncher(std::shared_ptr<IWindow>              window,
                    std::shared_ptr<IProjectLauncherView> view,
                    std::shared_ptr<IProjectLauncherData> data,
                    std::shared_ptr<IDwarfLogger>         logger);

    ProjectInformation
    Run() override;
  };
}