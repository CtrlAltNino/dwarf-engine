#pragma once

#include "IO/ISavedProjectsIO.h"
#include "ISavedProjects.h"
#include "Launcher/SavedProjects/IO/ISavedProjectsIO.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Launcher/SavedProjects/Sorter/ISavedProjectsSorter.h"
#include "Logging/IDwarfLogger.h"
#include "Project/IProjectSettingsIO.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include <memory>

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
    void
    AddProject(const std::filesystem::path& path) override;

    void
    OpenAddProjectDialog() override;

    void
    RemoveProject(const std::filesystem::path& path) override;

    void
    ChangeGraphicsApi(const std::filesystem::path& path,
                      GraphicsApi                  api) override;

    void
    RegisterProjectOpening(const std::filesystem::path& path) override;

    std::vector<SavedProject>&
    GetSavedProjects() override;
  };
}