#pragma once

#include "IProjectLauncherData.h"
#include "Launcher/IProjectLauncher.h"
#include "Launcher/ProjectList/IProjectList.h"
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
    std::unique_ptr<IProjectLauncherView> m_View;
    std::shared_ptr<IProjectLauncherData> m_Data;
    std::shared_ptr<IDwarfLogger>         m_Logger;
    std::shared_ptr<IProjectList>         m_ProjectList;

  public:
    ProjectLauncher(std::unique_ptr<IProjectLauncherView> view,
                    std::shared_ptr<IProjectLauncherData> data,
                    std::shared_ptr<IDwarfLogger>         logger,
                    std::shared_ptr<IProjectList>         projectList);

    ~ProjectLauncher() override = default;

    ProjectInformation
    Run() override;
  };
}