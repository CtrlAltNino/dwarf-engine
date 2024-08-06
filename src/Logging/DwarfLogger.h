#pragma once

#include "IDwarfLogger.h"
#include <spdlog/spdlog.h>

namespace Dwarf
{
  /**
   * @brief Default logger implementation.
   */
  class DwarfLogger : public IDwarfLogger
  {
  private:
    std::string                     m_Scope;
    std::shared_ptr<spdlog::logger> m_Logger;

    void
    LogMessage(spdlog::level::level_enum log_level, const Log& log) const;

  public:
    DwarfLogger();
    ~DwarfLogger() override;

    /**
     * @brief Log a message with the debug log level.
     * @param message Message to log.
     */
    void
    LogDebug(const Log logMessage) const override;

    /**
     * @brief Log a message with the info log level.
     * @param message Message to log.
     */
    void
    LogInfo(const Log logMessage) const override;

    /**
     * @brief Log a message with the warning log level.
     * @param message Message to log.
     */
    void
    LogWarn(const Log logMessage) const override;

    /**
     * @brief Log a message with the error log level.
     * @param message Message to log.
     */
    void
    LogError(const Log logMessage) const override;
  };
}