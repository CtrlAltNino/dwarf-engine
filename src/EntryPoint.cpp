#include "Application/DwarfEngine.h"

auto
main() -> int
{
  Dwarf::DwarfEngine engine = Dwarf::DwarfEngine();
  engine.run();
  return 0;
}