#pragma once

#include "Launcher/ProjectCreator/IProjectCreator.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Logging/IDwarfLogger.h"
#include "Project/IProjectSettingsIO.h"
#include "Utilities/FileHandler/IFileHandler.h"

namespace Dwarf
{
  class ProjectCreator : public IProjectCreator
  {
  private:
    std::shared_ptr<IDwarfLogger>       m_Logger;
    std::shared_ptr<ISavedProjects>     m_SavedProjects;
    std::shared_ptr<IFileHandler>       m_FileHandler;
    std::shared_ptr<IProjectSettingsIO> m_ProjectSettingsIO;

  public:
    ProjectCreator(std::shared_ptr<IDwarfLogger>       logger,
                   std::shared_ptr<ISavedProjects>     savedProjects,
                   std::shared_ptr<IFileHandler>       fileHandler,
                   std::shared_ptr<IProjectSettingsIO> projectSettingsIO);

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