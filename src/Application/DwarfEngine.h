#pragma once
#include "Core/Base.h"

namespace Dwarf
{
  /**
   * @brief Main engine class for the Dwarf Engine
   */
  class DwarfEngine
  {
  public:
    DwarfEngine() = default;
    ~DwarfEngine() = default;

    /**
     * @brief Run the engine
     */
    void
    Run();
  };
}