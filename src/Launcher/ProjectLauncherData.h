#pragma once

#include "Launcher/IProjectLauncherData.h"

namespace Dwarf
{
  class ProjectLauncherData : public IProjectLauncherData
  {
  private:
    ProjectChooserState m_State;
    ProjectInformation  m_SelectedProject;

  public:
    ProjectLauncherData();
    ~ProjectLauncherData() override = default;

    ProjectChooserState
    GetState() const override;

    void
    SetState(ProjectChooserState state) override;

    void
    SetSelectedProject(const ProjectInformation& project) override;

    const ProjectInformation&
    GetSelectedProject() const override;
  };
}