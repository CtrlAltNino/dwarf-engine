#pragma once

#include "Launcher/ProjectList/IO/IProjectListIO.h"

namespace Dwarf
{
  class ProjectListIO : public IProjectListIO
  {
  public:
    virtual std::vector<ProjectInformation>
    LoadProjectList() const override;

    virtual void
    SaveProjectList(
      const std::vector<ProjectInformation>& projectList) const override;
  };
}