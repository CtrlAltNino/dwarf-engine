#pragma once

namespace Dwarf
{
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
    LogDebug(const std::string& message) const = 0;

    /**
     * @brief Log a message with the info log level.
     * @param message Message to log.
     */
    virtual void
    LogInfo(const std::string& message) const = 0;

    /**
     * @brief Log a message with the warning log level.
     * @param message Message to log.
     */
    virtual void
    LogWarn(const std::string& message) const = 0;

    /**
     * @brief Log a message with the error log level.
     * @param message Message to log.
     */
    virtual void
    LogError(const std::string& message) const = 0;
  };
}