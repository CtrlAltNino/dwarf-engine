#pragma once
#include "Core/Base.h"
#include "Logging/DwarfLogger.h"
#include "Launcher/IProjectLauncher.h"
#include "Editor/IEditor.h"

namespace Dwarf
{
  /**
   * @brief Main engine class for the Dwarf Engine
   */
  class DwarfEngine
  {
  private:
    std::shared_ptr<DwarfLogger> m_Logger;

    ProjectInformation
    RunLauncher();

    bool
    RunEditor(ProjectInformation projectInformation);

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