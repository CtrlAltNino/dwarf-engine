#pragma once

#include "Launcher/ProjectList/IO/IProjectListIO.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class ProjectListIO : public IProjectListIO
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;

  public:
    ProjectListIO(std::shared_ptr<IDwarfLogger> logger);

    ~ProjectListIO() override;

    virtual std::vector<ProjectInformation>
    LoadProjectList() const override;

    virtual ProjectInformation
    LoadProject(const std::filesystem::path& path) const override;

    virtual void
    SaveProjectList(
      const std::vector<ProjectInformation>& projectList) const override;
  };
}