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

    virtual std::vector<SavedProject>
    LoadSavedProjects() const override;

    virtual void
    SaveSavedProjects(
      const std::vector<SavedProject>& savedProjects) const override;
  };
}