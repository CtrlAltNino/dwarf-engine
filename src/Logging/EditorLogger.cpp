#include "pch.h"
#include "EditorLogger.h"
#include <spdlog/spdlog.h>

namespace Dwarf
{
  EditorLogger::EditorLogger()
  {
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("%^[%T] %n: %v%$");
  }

  EditorLogger::~EditorLogger() = default;

  void
  EditorLogger::LogDebug(const std::string& message) const
  {
    spdlog::debug(message);
  }

  void
  EditorLogger::LogInfo(const std::string& message) const
  {
    spdlog::info(message);
  }

  void
  EditorLogger::LogWarn(const std::string& message) const
  {
    spdlog::warn(message);
  }

  void
  EditorLogger::LogError(const std::string& message) const
  {
    spdlog::error(message);
  }
}