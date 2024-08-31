#pragma once

#include "Launcher/IProjectLauncher.h"
#include "Project/ProjectTypes.h"
#include "Launcher/View/IProjectLauncherView.h"
#include "Window/IWindow.h"
#include "pch.h"

namespace Dwarf
{
  class ProjectLauncher : public IProjectLauncher
  {
  private:
    ProjectInformation                    m_SelectedProject;
    std::shared_ptr<IProjectLauncherView> m_View;
    std::shared_ptr<IWindow>              m_Window;
    ProjectChooserState                   m_State;

  public:
    ProjectLauncher(std::shared_ptr<IWindow>              window,
                    std::shared_ptr<IProjectLauncherView> view);

    ProjectPath
    Run() override;

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