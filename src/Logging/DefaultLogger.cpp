
#include "DefaultLogger.h"
#include <spdlog/spdlog.h>

namespace Dwarf
{
  DefaultLogger::DefaultLogger()
  {
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("%^[%T] %n: %v%$");
  }

  DefaultLogger::~DefaultLogger() = default;

  void
  DefaultLogger::LogDebug(const std::string& message) const
  {
    spdlog::debug(message);
  }

  void
  DefaultLogger::LogInfo(const std::string& message) const
  {
    spdlog::info(message);
  }

  void
  DefaultLogger::LogWarn(const std::string& message) const
  {
    spdlog::warn(message);
  }

  void
  DefaultLogger::LogError(const std::string& message) const
  {
    spdlog::error(message);
  }
}