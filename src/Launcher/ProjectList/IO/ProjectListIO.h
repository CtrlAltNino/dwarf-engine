#pragma once

#include "Launcher/ProjectList/IO/IProjectListIO.h"

namespace Dwarf
{
  class ProjectListIO : public IProjectListIO
  {
  public:
    ProjectListIO() = default;

    ~ProjectListIO() override = default;

    virtual std::vector<ProjectInformation>
    LoadProjectList() const override;

    virtual ProjectInformation
    LoadProject(const std::filesystem::path& path) const override;

    virtual void
    SaveProjectList(
      const std::vector<ProjectInformation>& projectList) const override;
  };
}