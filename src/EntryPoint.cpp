#include "pch.h"
#include "Logging/DwarfLogger.h"
#include "Application/DwarfEngine.h"
#include <spdlog/spdlog.h>
#define BOOST_DI_CFG_CTOR_LIMIT_SIZE 20
#include <boost/di.hpp>

int
main()
{
  // const auto injector = boost::di::make_injector();
  // const auto logger = injector.create<Dwarf::DwarfLogger>();

  // logger.LogInfo(Dwarf::Log("Starting Dwarf Engine...", "EntryPoint"));
  Dwarf::DwarfEngine engine = Dwarf::DwarfEngine();
  // logger.LogInfo(Dwarf::Log("Dwarf Engine instance created.", "EntryPoint"));

  // logger.LogInfo(Dwarf::Log("Running Dwarf Engine...", "EntryPoint"));
  engine.Run();
  // logger.LogInfo(Dwarf::Log("Dwarf Engine finished running.", "EntryPoint"));

  // logger.LogInfo(Dwarf::Log("Exiting Dwarf Engine...", "EntryPoint"));
  return 0;
}