#pragma once

#include "Launcher/SavedProjects/IO/ISavedProjectsIO.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler/IFileHandler.h"

namespace Dwarf
{
  class SavedProjectsIO : public ISavedProjectsIO
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    std::shared_ptr<IFileHandler> mFileHandler;

  public:
    SavedProjectsIO(std::shared_ptr<IDwarfLogger> logger,
                    std::shared_ptr<IFileHandler> fileHandler);

    ~SavedProjectsIO() override;

    /**
     * @brief Loads the saved projects list
     *
     * @return List of saved projects
     */
    [[nodiscard]] auto
    LoadSavedProjects() const -> std::vector<SavedProject> override;

    /**
     * @brief Writes a list of saved projects
     *
     * @param savedProjects List to write
     */
    void
    SaveSavedProjects(
      const std::vector<SavedProject>& savedProjects) const override;
  };
}