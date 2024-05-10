#include "Logging/DefaultLogger.h"
#include "Application/DwarfEngine.h"
#include <spdlog/spdlog.h>
#include <boost/di.hpp>

int
main()
{
  const auto injector = boost::di::make_injector();
  const auto logger = injector.create<Dwarf::DefaultLogger>();

  logger.LogInfo("Starting Dwarf Engine...");
  Dwarf::DwarfEngine engine = Dwarf::DwarfEngine();
  logger.LogInfo("Dwarf Engine instance created.");

  logger.LogInfo("Running Dwarf Engine...");
  engine.Run();
  logger.LogInfo("Dwarf Engine finished running.");

  logger.LogInfo("Exiting Dwarf Engine...");
  return 0;
}