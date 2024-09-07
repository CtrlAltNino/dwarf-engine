#include "pch.h"
#include "Logging/DwarfLogger.h"
#include "Application/DwarfEngine.h"

int
main()
{
  const auto logger = Dwarf::DwarfLogger(Dwarf::LogName("EntryPoint"));

  logger.LogInfo(Dwarf::Log("Starting Dwarf Engine...", "EntryPoint"));
  Dwarf::DwarfEngine engine = Dwarf::DwarfEngine();
  logger.LogInfo(Dwarf::Log("Dwarf Engine instance created.", "EntryPoint"));

  logger.LogInfo(Dwarf::Log("Running Dwarf Engine...", "EntryPoint"));
  engine.Run();
  logger.LogInfo(Dwarf::Log("Dwarf Engine finished running.", "EntryPoint"));

  logger.LogInfo(Dwarf::Log("Exiting Application...", "EntryPoint"));
  return 0;
}