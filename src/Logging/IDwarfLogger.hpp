#pragma once

namespace Dwarf
{
  struct Log
  {
    std::string Message;
    std::string Scope;

    Log(std::string message, std::string scope)
      : Message(std::move(message))
      , Scope(std::move(scope))
    {
    }

    // Comparison operator for testing purposes.
    auto
    operator==(const Log& other) const -> bool
    {
      return Message == other.Message && Scope == other.Scope;
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
    LogDebug(Log logMessage) const = 0;

    /**
     * @brief Log a message with the info log level.
     * @param message Message to log.
     */
    virtual void
    LogInfo(Log logMessage) const = 0;

    /**
     * @brief Log a message with the warning log level.
     * @param message Message to log.
     */
    virtual void
    LogWarn(Log logMessage) const = 0;

    /**
     * @brief Log a message with the error log level.
     * @param message Message to log.
     */
    virtual void
    LogError(Log logMessage) const = 0;
  };
}