#include "pch.h"

#include "DwarfLogger.h"
#include "IDwarfLogger.h"
#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace Dwarf
{
  DwarfLogger::DwarfLogger(const LogName& logName)
  {
    // Create a console sink (colored output)
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    // Create a file sink (logs to a file)
    auto file_sink =
      std::make_shared<spdlog::sinks::basic_file_sink_mt>(LOG_FILE_NAME, true);

    // Create a logger with multiple sinks
    mLogger = std::make_shared<spdlog::logger>(
      "multi_sink", spdlog::sinks_init_list{ console_sink, file_sink });

    if (std::string(LOG_FILE_NAME).find("debug") != std::string::npos)
    {
      mLogger->set_level(spdlog::level::debug);
    }
    else
    {
      mLogger->set_level(spdlog::level::info);
    }

    spdlog::set_default_logger(mLogger);

    spdlog::flush_every(std::chrono::seconds(3));

    mLogger->log(spdlog::level::info,
                 fmt::format("[{}] {}", "DwarfLogger", "Logger initialized."));
  }

  DwarfLogger::~DwarfLogger()
  {
    if (mLogger)
    {
      mLogger->log(spdlog::level::info,
                   fmt::format("[{}] {}", "DwarfLogger", "Logger destroyed."));
    }

    spdlog::shutdown();
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
    mLogger->flush();
  }

  void
  DwarfLogger::LogMessage(spdlog::level::level_enum log_level,
                          const Log&                log) const
  {
    mLogger->log(log_level,
                 fmt::format("[{}] {}", log.Scope.c_str(), log.Message));
  }
}