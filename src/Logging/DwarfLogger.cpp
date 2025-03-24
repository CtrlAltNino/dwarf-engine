#include "pch.h"

#include "DwarfLogger.h"
#include "IDwarfLogger.h"
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Dwarf
{
  DwarfLogger::DwarfLogger(const LogName& logName)
    : mLogger(spdlog::stdout_color_mt(logName.t))
  {
    mLogger->set_level(spdlog::level::trace);
    mLogger->log(spdlog::level::info,
                 fmt::format("[{}] {}", "DwarfLogger", "Logger initialized."));
  }

  DwarfLogger::~DwarfLogger()
  {
    if (mLogger)
    {
      mLogger->log(spdlog::level::info,
                   fmt::format("[{}] {}", "DwarfLogger", "Logger destroyed."));
      mLogger->flush();
    }

    spdlog::drop(mLogger->name());
  }

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
    mLogger->log(log_level,
                 fmt::format("[{}] {}", log.Scope.c_str(), log.Message));
    // mLogger->log(
    //   log_level,
    //   log.Color.has_value()
    //     ? fmt::format(
    //         fg(fmt::color::red), "[{}] {}", log.Scope.c_str(), log.Message)
    //     : fmt::format("[{}] {}", log.Scope.c_str(), log.Message));
  }
}