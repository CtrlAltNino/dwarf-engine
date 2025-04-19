#pragma once

#include "Launcher/SavedProjects/ISavedProjects.hpp"
#include "Logging/DwarfLogger.hpp"

namespace Dwarf
{
  /**
   * @brief Main engine class for the Dwarf Engine
   *
   */
  class DwarfEngine
  {
  private:
    std::shared_ptr<DwarfLogger> mLogger;

    /**
     * @brief Runs the project launcher loop
     *
     * @return The saved data of the project if the user chose one
     */
    auto
    RunLauncher() -> std::optional<SavedProject>;

    /**
     * @brief Runs the editor loop
     *
     * @param savedProject The saved project data
     * @return true The editor should return to the project launcher
     * @return false The application should close
     */
    auto
    RunEditor(const SavedProject& savedProject) -> bool;

  public:
    DwarfEngine();

    /**
     * @brief Run the engine
     */
    void
    Run();
  };
}