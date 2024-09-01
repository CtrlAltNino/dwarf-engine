#pragma once

#include "Launcher/IProjectLauncher.h"
namespace Dwarf
{
  class IProjectListIO
  {
  public:
    virtual std::vector<ProjectInformation>
    LoadProjectList() const = 0;

    virtual ProjectInformation
    LoadProject(const std::filesystem::path& path) const = 0;

    virtual void
    SaveProjectList(
      const std::vector<ProjectInformation>& projectList) const = 0;
  };
}