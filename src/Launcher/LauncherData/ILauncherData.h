#pragma once

#include "Launcher/IProjectLauncher.h"
#include "Launcher/SavedProjects/ISavedProjects.h"

namespace Dwarf
{
  /**
   * @brief Class containing the relevant state data of the project launcher
   *
   */
  class ILauncherData
  {
  public:
    virtual ~ILauncherData() = default;

    /**
     * @brief Returns the current state of the launcher
     *
     * @return Enum value representing the state
     */
    [[nodiscard]] virtual auto
    GetState() const -> ProjectChooserState = 0;

    /**
     * @brief Sets the state of the launcher
     *
     * @param state Enum value representing the state
     */
    virtual void
    SetState(ProjectChooserState state) = 0;

    /**
     * @brief Sets the selected project (For choosing it or manipulating it)
     *
     * @param project Saved project data to be selected
     */
    virtual void
    SetSelectedProject(std::optional<SavedProject> project) = 0;

    /**
     * @brief Returns the currently selected project
     *
     * @return Currently selected project
     */
    [[nodiscard]] virtual auto
    GetSelectedProject() const -> const std::optional<SavedProject>& = 0;
  };
} // namespace Dwarf