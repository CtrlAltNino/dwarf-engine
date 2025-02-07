#pragma once
#include <fmt/color.h>
#include <optional>

namespace Dwarf
{
  struct Log
  {
    const std::string               Message;
    const std::string               Scope;
    const std::optional<fmt::color> Color;

    Log(const std::string&        message,
        const std::string&        scope,
        std::optional<fmt::color> color = std::nullopt)
      : Message(message)
      , Scope(scope)
      , Color(color)
    {
    }

    // Comparison operator for testing purposes.
    bool
    operator==(const Log& other) const
    {
      return Message == other.Message && Scope == other.Scope &&
             Color == other.Color;
    }
  };

  /// @brief Interface for logging messages with different log levels.
  class IDwarfLogger
  {
  public:
    virtual ~IDwarfLogger() = default;

    /**
     * @brief Log a message with the debug log level.
     * @param message Message to log.
     */
    virtual void
    LogDebug(const Log logMessage) const = 0;

    /**
     * @brief Log a message with the info log level.
     * @param message Message to log.
     */
    virtual void
    LogInfo(const Log logMessage) const = 0;

    /**
     * @brief Log a message with the warning log level.
     * @param message Message to log.
     */
    virtual void
    LogWarn(const Log logMessage) const = 0;

    /**
     * @brief Log a message with the error log level.
     * @param message Message to log.
     */
    virtual void
    LogError(const Log logMessage) const = 0;
  };
}