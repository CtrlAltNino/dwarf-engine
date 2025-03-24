#pragma once

#include "IO/ISavedProjectsIO.h"
#include "ISavedProjects.h"
#include "Launcher/SavedProjects/IO/ISavedProjectsIO.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Launcher/SavedProjects/Sorter/ISavedProjectsSorter.h"
#include "Logging/IDwarfLogger.h"
#include "Project/IProjectSettingsIO.h"

namespace Dwarf
{
  class SavedProjects : public ISavedProjects
  {
  private:
    std::vector<SavedProject>             mSavedProjects;
    std::shared_ptr<IDwarfLogger>         mLogger;
    std::shared_ptr<ISavedProjectsIO>     mSavedProjectsIO;
    std::shared_ptr<ISavedProjectsSorter> mSavedProjectsSorter;
    std::shared_ptr<IProjectSettingsIO>   mProjectSettingsIO;

  public:
    SavedProjects(std::shared_ptr<IDwarfLogger>         logger,
                  std::shared_ptr<ISavedProjectsIO>     savedProjectsIO,
                  std::shared_ptr<ISavedProjectsSorter> savedProjectsSorter,
                  std::shared_ptr<IProjectSettingsIO>   projectSettingsIO);

    ~SavedProjects() override;

    /**
     * @brief Adds a project to the list by path
     *
     * @param path Path to a project directory
     */
    void
    AddProject(const std::filesystem::path& path) override;

    /**
     * @brief Opens a dialog to find and add a project
     *
     */
    void
    OpenAddProjectDialog() override;

    /**
     * @brief Removes a project from the list
     *
     * @param path Path to a project directory
     */
    void
    RemoveProject(const std::filesystem::path& path) override;

    /**
     * @brief Changes the graphics api of a project
     *
     * @param path Path to the project directory
     * @param api Api to change the project to
     */
    void
    ChangeGraphicsApi(const std::filesystem::path& path,
                      GraphicsApi                  api) override;

    /**
     * @brief Updates the time stamp for the last time a project was opened
     *
     * @param path Path to a project directory
     */
    void
    RegisterProjectOpening(const std::filesystem::path& path) override;

    /**
     * @brief Returns the list of saved projects
     *
     * @return List of saved projects
     */
    [[nodiscard]] auto
    GetSavedProjects() -> std::vector<SavedProject>& override;
  };
}