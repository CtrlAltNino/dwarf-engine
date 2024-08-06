#include "pch.h"
#include "DwarfLogger.h"
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Dwarf
{
  DwarfLogger::DwarfLogger()
    : m_Logger(spdlog::stdout_color_mt("Dwarf"))
  {
  }

  DwarfLogger::~DwarfLogger() = default;

  void
  DwarfLogger::LogDebug(const Log logMessage) const
  {
    LogMessage(spdlog::level::debug, logMessage);
  }

  void
  DwarfLogger::LogInfo(const Log logMessage) const
  {
    LogMessage(spdlog::level::info, logMessage);
  }

  void
  DwarfLogger::LogWarn(const Log logMessage) const
  {
    LogMessage(spdlog::level::warn, logMessage);
  }

  void
  DwarfLogger::LogError(const Log logMessage) const
  {
    LogMessage(spdlog::level::err, logMessage);
  }

  void
  DwarfLogger::LogMessage(spdlog::level::level_enum log_level,
                          const Log&                log) const
  {
    std::string formatted_message =
      fmt::format("[{}] {}", log.Scope, log.Message);

    if (log.Color.has_value())
    {
      formatted_message =
        fmt::format(fmt::fg(log.Color.value()), formatted_message);
    }

    m_Logger->log(log_level, formatted_message);
  }
}