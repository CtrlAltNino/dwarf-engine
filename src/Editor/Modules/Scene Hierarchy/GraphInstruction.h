#pragma once

#include "Core/Scene/Scene.h"

namespace Dwarf {

  /// @brief Base graph instruction class.
  class GraphInstruction
  {
  public:
    GraphInstruction() = default;
    virtual ~GraphInstruction() = default;

    /// @brief Executes the instruction.
    virtual void PerformInstruction() = 0;
  };
}