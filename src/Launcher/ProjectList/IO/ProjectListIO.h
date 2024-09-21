#pragma once

#include "Launcher/ProjectList/IO/IProjectListIO.h"
#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler/IFileHandler.h"

namespace Dwarf
{
  class ProjectListIO : public IProjectListIO
  {
  private:
    IDwarfLogger&                 m_Logger;
    std::shared_ptr<IFileHandler> m_FileHandler;

  public:
    ProjectListIO(IDwarfLogger&                 logger,
                  std::shared_ptr<IFileHandler> fileHandler);

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