#pragma once

#include "Launcher/SavedProjects/ISavedProjects.h"

namespace Dwarf
{
  /**
   * @brief Class that manages to read and write the saved projects file
   *
   */
  class ISavedProjectsIO
  {
  public:
    virtual ~ISavedProjectsIO() = default;

    /**
     * @brief Loads the saved projects list
     *
     * @return List of saved projects
     */
    [[nodiscard]] virtual auto
    LoadSavedProjects() const -> std::vector<SavedProject> = 0;

    /**
     * @brief Writes a list of saved projects
     *
     * @param savedProjects List to write
     */
    virtual void
    SaveSavedProjects(const std::vector<SavedProject>& savedProjects) const = 0;
  };
}