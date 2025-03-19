#include "Application/DwarfEngine.h"

auto
main() -> int
{
  Dwarf::DwarfEngine engine = Dwarf::DwarfEngine();
  engine.Run();
  return 0;
}