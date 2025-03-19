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
    std::shared_ptr<DwarfLogger> m_Logger;

    auto
    runLauncher() -> std::optional<SavedProject>;

    auto
    runEditor(const SavedProject& savedProject) -> bool;

  public:
    DwarfEngine();
    //~DwarfEngine() = default;

    /**
     * @brief Run the engine
     */
    void
    run();
  };
}