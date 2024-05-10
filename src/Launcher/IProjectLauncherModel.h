#pragma once

#include "Core/Base.h"
#include "Launcher/ProjectLauncherUtilities.h"

namespace Dwarf
{
  /**
   * @brief Interface for the project launcher model
   */
  class IProjectLauncherModel
  {
  public:
    virtual ~IProjectLauncherModel() = default;

    /**
     * @brief Get the current state of the project launcher
     * @return Current state of the project launcher
     */
    virtual ProjectChooserState
    GetState() const = 0;

    /**
     * @brief Set the state of the project launcher
     * @param state New state of the project launcher
     */
    virtual void
    SetState(ProjectChooserState state) = 0;

    /**
     * @brief Get the ID of the selected project
     * @return ID of the selected project
     */
    virtual int
    GetSelectedProjectID() const = 0;

    /**
     * @brief Set the ID of the selected project
     * @param id ID of the selected project
     */
    virtual void
    SetSelectedProjectID(int id) = 0;
  };
}