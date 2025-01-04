#pragma once

#include "Launcher/IProjectLauncher.h"
#include "SavedProjects/ISavedProjects.h"
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
    SetSelectedProject(std::optional<SavedProject> project) = 0;

    virtual const std::optional<SavedProject>&
    GetSelectedProject() const = 0;
  };
} // namespace Dwarf