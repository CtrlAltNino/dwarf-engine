#pragma once

#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  /**
   * @brief Default logger implementation.
   */
  class EditorLogger : public IDwarfLogger
  {
  public:
    EditorLogger();
    ~EditorLogger() override;

    /**
     * @brief Log a message with the debug log level.
     * @param message Message to log.
     */
    void
    LogDebug(const std::string& message) const override;

    /**
     * @brief Log a message with the info log level.
     * @param message Message to log.
     */
    void
    LogInfo(const std::string& message) const override;

    /**
     * @brief Log a message with the warning log level.
     * @param message Message to log.
     */
    void
    LogWarn(const std::string& message) const override;

    /**
     * @brief Log a message with the error log level.
     * @param message Message to log.
     */
    void
    LogError(const std::string& message) const override;
  };
}