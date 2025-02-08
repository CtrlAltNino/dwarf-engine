#pragma once

#include "pch.h"
#include "Launcher/LauncherData/ILauncherData.h"
#include "Launcher/IProjectLauncher.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Logging/IDwarfLogger.h"
#include "Launcher/View/IProjectLauncherView.h"

namespace Dwarf
{
  class ProjectLauncher : public IProjectLauncher
  {
  private:
    std::unique_ptr<IProjectLauncherView> m_View;
    std::shared_ptr<ILauncherData>        m_Data;
    std::shared_ptr<ISavedProjects>       m_SavedProjects;
    std::shared_ptr<IDwarfLogger>         m_Logger;

  public:
    ProjectLauncher(std::shared_ptr<IDwarfLogger>         logger,
                    std::unique_ptr<IProjectLauncherView> view,
                    std::shared_ptr<ILauncherData>        data,
                    std::shared_ptr<ISavedProjects>       projectList);

    ~ProjectLauncher() override;

    std::optional<SavedProject>
    Run() override;
  };
}