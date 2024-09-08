#pragma once

#include "Launcher/ProjectCreator/IProjectCreator.h"
#include "Launcher/ProjectList/IProjectList.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class ProjectCreator : public IProjectCreator
  {
  private:
    IDwarfLogger& m_Logger;
    IProjectList& m_ProjectList;

  public:
    ProjectCreator(IDwarfLogger& logger, IProjectList& projectList);

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