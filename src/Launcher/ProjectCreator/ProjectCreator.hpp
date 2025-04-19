#pragma once

#include "Launcher/ProjectCreator/IProjectCreator.hpp"
#include "Launcher/SavedProjects/ISavedProjects.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "Project/IProjectSettingsIO.hpp"
#include "Utilities/FileHandler/IFileHandler.hpp"

namespace Dwarf
{
  class ProjectCreator : public IProjectCreator
  {
  private:
    std::shared_ptr<IDwarfLogger>       mLogger;
    std::shared_ptr<ISavedProjects>     mSavedProjects;
    std::shared_ptr<IFileHandler>       mFileHandler;
    std::shared_ptr<IProjectSettingsIO> mProjectSettingsIO;

  public:
    ProjectCreator(std::shared_ptr<IDwarfLogger>       logger,
                   std::shared_ptr<ISavedProjects>     savedProjects,
                   std::shared_ptr<IFileHandler>       fileHandler,
                   std::shared_ptr<IProjectSettingsIO> projectSettingsIO);

    ~ProjectCreator() override;

    /**
     * @brief Gets the default project path
     *
     * @return Path to the default location to create projects at
     */
    [[nodiscard]] auto
    GetDefaultProjectPath() const -> std::filesystem::path override;

    /**
     * @brief Creates a new project with the provided properties
     *
     * @param projectName Name of the project
     * @param projectPath Directory path
     * @param graphicsApi Graphics api to use
     * @param projectTemplate Project template to base the project off of
     */
    void
    CreateProject(std::string           projectName,
                  std::filesystem::path projectPath,
                  GraphicsApi           graphicsApi,
                  ProjectTemplate       projectTemplate) override;
  };
}