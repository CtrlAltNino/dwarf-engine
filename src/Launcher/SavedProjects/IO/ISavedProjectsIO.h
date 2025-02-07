#pragma once

#include "Launcher/IProjectLauncher.h"
#include "Launcher/SavedProjects/ISavedProjects.h"

namespace Dwarf
{
  class ISavedProjectsIO
  {
  public:
    virtual ~ISavedProjectsIO() = default;

    virtual std::vector<SavedProject>
    LoadSavedProjects() const = 0;

    virtual void
    SaveSavedProjects(const std::vector<SavedProject>& savedProjects) const = 0;
  };
}