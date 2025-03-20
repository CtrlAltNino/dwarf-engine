#pragma once

#include "Launcher/IProjectLauncher.h"
#include "Launcher/LauncherData/ILauncherData.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Launcher/View/IProjectLauncherView.h"
#include "Logging/IDwarfLogger.h"
#include "pch.h"

namespace Dwarf
{
  class ProjectLauncher : public IProjectLauncher
  {
  private:
    std::unique_ptr<IProjectLauncherView> mView;
    std::shared_ptr<ILauncherData>        mData;
    std::shared_ptr<ISavedProjects>       mSavedProjects;
    std::shared_ptr<IDwarfLogger>         mLogger;

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