#pragma once

#include "Utilities/TimeUtilities.h"

namespace Dwarf
{
  /**
   * @brief Class that provides statistics about the editor runtime
   *
   */
  class IEditorStats
  {
  public:
    virtual ~IEditorStats() = default;

    /**
     * @brief Returns the TimeStamp of the current frame
     *
     * @return Const reference to the current frames TimeStamp
     */
    [[nodiscard]] virtual auto
    GetCurrentTimeStamp() const -> const TimeStamp& = 0;

    /**
     * @brief Sets the current frames time stamp
     *
     * @param timeStamp TimeStamp to set it to
     */
    virtual void
    SetCurrentTimeStamp(const TimeStamp& timeStamp) = 0;

    /**
     * @brief Returns the TimeStamp of the last frame
     *
     * @return Const reference to the last TimeStamp
     */
    [[nodiscard]] virtual auto
    GetLastTimeStamp() const -> const TimeStamp& = 0;

    /**
     * @brief Sets the TimeStamp of the last frame
     *
     * @param timeStamp TimeStamp to set to
     */
    virtual void
    SetLastTimeStamp(const TimeStamp& timeStamp) = 0;

    /**
     * @brief Returns the time elapsed since the last frame
     *
     * @return Time in miliseconds
     */
    [[nodiscard]] virtual auto
    GetDeltaTime() const -> double = 0;

    /**
     * @brief Sets if the editor should return to the project launcher
     *
     * @param returnToLauncher True if should return, false if the application
     * should close
     */
    virtual void
    SetReturnToLauncher(bool returnToLauncher) = 0;

    /**
     * @brief Returns if the editor should return to the launcher
     *
     * @return true If it should return
     * @return false If application should close
     */
    [[nodiscard]] virtual auto
    GetReturnToLauncher() const -> bool = 0;

    /**
     * @brief Sets if the editor should close
     *
     * @param closeSignal True if the editor should close, false if not
     */
    virtual void
    SetCloseSignal(bool closeSignal) = 0;

    /**
     * @brief Returns the stores close signal
     *
     * @return true If the editor is done running
     * @return false If not
     */
    [[nodiscard]] virtual auto
    GetCloseSignal() const -> bool = 0;

    /**
     * @brief Sets the rendering device info string
     *
     * @param deviceInfo To display the GPU and driver version
     */
    virtual void
    SetDeviceInfo(const std::string& deviceInfo) = 0;

    /**
     * @brief Returns the stores device info string
     *
     * @return Stores device info string
     */
    [[nodiscard]] virtual auto
    GetDeviceInfo() const -> const std::string& = 0;

    /**
     * @brief Set the Time Since Start object
     *
     * @param timeStamp
     */
    virtual void
    SetInitialTimeStamp(const TimeStamp& timeStamp) = 0;

    /**
     * @brief Returns the elapsed time since the start of the editor
     *
     * @return Time in seconds
     */
    [[nodiscard]] virtual auto
    GetTimeSinceStart() const -> double = 0;
  };
}