#pragma once

#include "Launcher/LauncherData/ILauncherData.h"
#include "Logging/IDwarfLogger.h"
#include "Launcher/SavedProjects/ISavedProjects.h"

namespace Dwarf
{
  class LauncherData : public ILauncherData
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;
    ProjectChooserState           m_State;
    std::optional<SavedProject>   m_SelectedProject;

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