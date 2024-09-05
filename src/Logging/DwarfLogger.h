#pragma once

#include "IDwarfLogger.h"
#include <boost/serialization/strong_typedef.hpp>
#include <spdlog/spdlog.h>

namespace Dwarf
{
  BOOST_STRONG_TYPEDEF(std::string, LogName)
  /**
   * @brief Default logger implementation.
   */
  class DwarfLogger : public IDwarfLogger
  {
  private:
    std::shared_ptr<spdlog::logger> m_Logger;

    void
    LogMessage(spdlog::level::level_enum log_level, const Log& log) const;

  public:
    BOOST_DI_INJECT(DwarfLogger, LogName logName);
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