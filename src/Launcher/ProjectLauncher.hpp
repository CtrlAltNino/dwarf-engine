#pragma once

#include "Launcher/IProjectLauncher.hpp"
#include "Launcher/LauncherData/ILauncherData.hpp"
#include "Launcher/SavedProjects/ISavedProjects.hpp"
#include "Launcher/View/IProjectLauncherView.hpp"
#include "Logging/IDwarfLogger.hpp"

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

    /**
     * @brief Runs the project launcher
     *
     * @return If the user chose a project, the saved project data will be
     * returned
     */
    auto
    Run() -> std::optional<SavedProject> override;
  };
}