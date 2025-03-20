#pragma once

#include "Launcher/LauncherData/ILauncherData.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class LauncherData : public ILauncherData
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    ProjectChooserState           mState;
    std::optional<SavedProject>   mSelectedProject;

  public:
    LauncherData(std::shared_ptr<IDwarfLogger> logger);
    ~LauncherData() override;

    ProjectChooserState
    GetState() const override;

    void
    SetState(ProjectChooserState state) override;

    void
    SetSelectedProject(std::optional<SavedProject> project) override;

    const std::optional<SavedProject>&
    GetSelectedProject() const override;
  };
}