#pragma once
#define SDL_MAIN_HANDLED
#include "Core/Base.h"

namespace Dwarf {

  class DwarfEngine
  {
  public:
    DwarfEngine();
    ~DwarfEngine();

    void Run();

  private:
    static DwarfEngine* s_Instance;
  };

  Ref<DwarfEngine> Create();
}