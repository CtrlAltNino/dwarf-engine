#pragma once

#include "Launcher/IProjectLauncher.h"
namespace Dwarf
{
  class IProjectLauncherData
  {
  public:
    virtual ~IProjectLauncherData() = default;

    virtual ProjectChooserState
    GetState() const = 0;

    virtual void
    SetState(ProjectChooserState state) = 0;

    virtual void
    SetSelectedProject(const ProjectInformation& project) = 0;

    virtual const ProjectInformation&
    GetSelectedProject() const = 0;
  };
} // namespace Dwarf