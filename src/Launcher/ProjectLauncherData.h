#pragma once

#include "Launcher/IProjectLauncherData.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class ProjectLauncherData : public IProjectLauncherData
  {
  private:
    IDwarfLogger&       m_Logger;
    ProjectChooserState m_State;
    ProjectInformation  m_SelectedProject;

  public:
    ProjectLauncherData(IDwarfLogger& logger);
    ~ProjectLauncherData() override;

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