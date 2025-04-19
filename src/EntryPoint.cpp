#include "pch.hpp"

#include "Application/DwarfEngine.hpp"

auto
main() -> int
{
  Dwarf::DwarfEngine engine = Dwarf::DwarfEngine();
  engine.Run();
  return 0;
}