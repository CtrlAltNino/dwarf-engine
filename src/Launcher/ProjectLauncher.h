#pragma once

#include "pch.h"
#include "IProjectLauncherData.h"
#include "Launcher/IProjectLauncher.h"
#include "Launcher/ProjectList/IProjectList.h"
#include "Logging/IDwarfLogger.h"
#include "Launcher/View/IProjectLauncherView.h"

namespace Dwarf
{
  class ProjectLauncher : public IProjectLauncher
  {
  private:
    std::unique_ptr<IProjectLauncherView> m_View;
    IProjectLauncherData&                 m_Data;
    IProjectList&                         m_ProjectList;
    IDwarfLogger&                         m_Logger;

  public:
    ProjectLauncher(IDwarfLogger&                         logger,
                    std::unique_ptr<IProjectLauncherView> view,
                    IProjectLauncherData&                 data,
                    IProjectList&                         projectList);

    ~ProjectLauncher() override;

    ProjectInformation
    Run() override;
  };
}