#pragma once

#include "IProjectSettings.h"

namespace Dwarf
{
  /**
   * @brief Provides functionality to read and write the project settings
   *
   */
  class IProjectSettingsIO
  {
  public:
    virtual ~IProjectSettingsIO() = default;

    /**
     * @brief Loads the project settings from a given project path
     *
     * @param projectPath Path to a project directory
     * @return Optional project settings struct if a file was found
     */
    virtual auto
    LoadProjectSettings(std::filesystem::path projectPath)
      -> std::optional<ProjectSettingsData> = 0;

    /**
     * @brief Writes project settings to a path
     *
     * @param projectSettingsData Project settings to write
     * @param projectPath Path to a project directory
     */
    virtual void
    SaveProjectSettings(ProjectSettingsData   projectSettingsData,
                        std::filesystem::path projectPath) = 0;
  };
}