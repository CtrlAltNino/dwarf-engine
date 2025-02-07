#pragma once

#include "Launcher/IProjectLauncherData.h"
#include "Logging/IDwarfLogger.h"
#include "SavedProjects/ISavedProjects.h"

namespace Dwarf
{
  class ProjectLauncherData : public IProjectLauncherData
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;
    ProjectChooserState           m_State;
    std::optional<SavedProject>   m_SelectedProject;

  public:
    ProjectLauncherData(std::shared_ptr<IDwarfLogger> logger);
    ~ProjectLauncherData() override;

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