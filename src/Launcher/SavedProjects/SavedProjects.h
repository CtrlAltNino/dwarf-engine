#pragma once

#include "IO/ISavedProjectsIO.h"
#include "ISavedProjects.h"
#include "Launcher/SavedProjects/IO/ISavedProjectsIO.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Launcher/SavedProjects/Sorter/ISavedProjectsSorter.h"
#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include "Project/IProjectSettingsIO.h"
#include <memory>

namespace Dwarf
{
  class SavedProjects : public ISavedProjects
  {
  private:
    std::vector<SavedProject>             m_SavedProjects;
    std::shared_ptr<IDwarfLogger>         m_Logger;
    std::shared_ptr<ISavedProjectsIO>     m_SavedProjectsIO;
    std::shared_ptr<ISavedProjectsSorter> m_SavedProjectsSorter;
    std::shared_ptr<IProjectSettingsIO>   m_ProjectSettingsIO;

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