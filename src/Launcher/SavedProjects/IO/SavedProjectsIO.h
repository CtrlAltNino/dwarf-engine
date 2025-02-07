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
    std::shared_ptr<IDwarfLogger> m_Logger;
    std::shared_ptr<IFileHandler> m_FileHandler;

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