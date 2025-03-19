#pragma once

#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Logging/DwarfLogger.h"

namespace Dwarf
{
  /**
   * @brief Main engine class for the Dwarf Engine
   */
  class DwarfEngine
  {
  private:
    std::shared_ptr<DwarfLogger> mLogger;

    auto
    RunLauncher() -> std::optional<SavedProject>;

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