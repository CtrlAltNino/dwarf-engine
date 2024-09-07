#pragma once

#include "Launcher/ProjectCreator/IProjectCreator.h"
#include "Launcher/ProjectList/IProjectList.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class ProjectCreator : public IProjectCreator
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;
    std::shared_ptr<IProjectList> m_ProjectList;

  public:
    ProjectCreator(std::shared_ptr<IDwarfLogger> logger,
                   std::shared_ptr<IProjectList> projectList);

    ~ProjectCreator() override;

    std::filesystem::path
    GetDefaultProjectPath() const override;

    void
    CreateProject(std::string           projectName,
                  std::filesystem::path projectPath,
                  GraphicsApi           graphicsApi,
                  ProjectTemplate       projectTemplate) override;
  };
}