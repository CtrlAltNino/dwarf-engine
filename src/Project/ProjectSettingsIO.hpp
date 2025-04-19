#pragma once

#include "IProjectSettings.hpp"
#include "IProjectSettingsIO.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "Utilities/FileHandler/IFileHandler.hpp"

namespace Dwarf
{
  class ProjectSettingsIO : public IProjectSettingsIO
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    std::shared_ptr<IFileHandler> mFileHandler;

  public:
    BOOST_DI_INJECT(ProjectSettingsIO,
                    std::shared_ptr<IDwarfLogger> logger,
                    std::shared_ptr<IFileHandler> fileHandler);
    ~ProjectSettingsIO() override = default;

    /**
     * @brief Loads the project settings from a given project path
     *
     * @param projectPath Path to a project directory
     * @return Optional project settings struct if a file was found
     */
    auto
    LoadProjectSettings(std::filesystem::path projectPath)
      -> std::optional<ProjectSettingsData> override;

    /**
     * @brief Writes project settings to a path
     *
     * @param projectSettingsData Project settings to write
     * @param projectPath Path to a project directory
     */
    void
    SaveProjectSettings(ProjectSettingsData   projectSettingsData,
                        std::filesystem::path projectPath) override;
  };
}