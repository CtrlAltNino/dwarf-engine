#pragma once
#include "Logging/DwarfLogger.h"
#include "Launcher/SavedProjects/ISavedProjects.h"

namespace Dwarf
{
  /**
   * @brief Main engine class for the Dwarf Engine
   */
  class DwarfEngine
  {
  private:
    std::shared_ptr<DwarfLogger> m_Logger;

    std::optional<SavedProject>
    RunLauncher();

    bool
    RunEditor(SavedProject savedProject);

  public:
    DwarfEngine();
    ~DwarfEngine() = default;

    /**
     * @brief Run the engine
     */
    void
    Run();
  };
}