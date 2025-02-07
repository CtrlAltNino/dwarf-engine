#pragma once

#include "IProjectSettings.h"
#include "IProjectSettingsIO.h"
#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler/IFileHandler.h"

namespace Dwarf
{
  class ProjectSettingsIO : public IProjectSettingsIO
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;
    std::shared_ptr<IFileHandler> m_FileHandler;

  public:
    BOOST_DI_INJECT(ProjectSettingsIO,
                    std::shared_ptr<IDwarfLogger> logger,
                    std::shared_ptr<IFileHandler> fileHandler);
    ~ProjectSettingsIO() override = default;

    std::optional<ProjectSettingsData>
    LoadProjectSettings(std::filesystem::path projectPath) override;

    void
    SaveProjectSettings(ProjectSettingsData   projectSettingsData,
                        std::filesystem::path projectPath) override;
  };
}