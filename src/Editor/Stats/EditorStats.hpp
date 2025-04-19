#pragma once

#include "Editor/Stats/IEditorStats.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "Utilities/TimeUtilities.hpp"

namespace Dwarf
{
  class EditorStats : public IEditorStats
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    TimeStamp                     mCurrentTimeStamp{ 0 };
    TimeStamp                     mLastTimeStamp{ 0 };
    TimeStamp                     mInitialTimeStamp{ 0 };
    std::string                   mDeviceInfo;
    bool                          mReturnToLauncher = false;
    bool                          mCloseSignal = false;

  public:
    EditorStats(std::shared_ptr<IDwarfLogger> logger);
    ~EditorStats() override;

    /**
     * @brief Returns the TimeStamp of the current frame
     *
     * @return Const reference to the current frames TimeStamp
     */
    [[nodiscard]] auto
    GetCurrentTimeStamp() const -> const TimeStamp& override;

    /**
     * @brief Sets the current frames time stamp
     *
     * @param timeStamp TimeStamp to set it to
     */
    void
    SetCurrentTimeStamp(const TimeStamp& timeStamp) override;

    /**
     * @brief Returns the TimeStamp of the last frame
     *
     * @return Const reference to the last TimeStamp
     */
    [[nodiscard]] auto
    GetLastTimeStamp() const -> const TimeStamp& override;

    /**
     * @brief Sets the TimeStamp of the last frame
     *
     * @param timeStamp TimeStamp to set to
     */
    void
    SetLastTimeStamp(const TimeStamp& timeStamp) override;

    /**
     * @brief Returns the time elapsed since the last frame
     *
     * @return Time in miliseconds
     */
    [[nodiscard]] auto
    GetDeltaTime() const -> double override;

    /**
     * @brief Sets if the editor should return to the project launcher
     *
     * @param returnToLauncher True if should return, false if the application
     * should close
     */
    void
    SetReturnToLauncher(bool returnToLauncher) override;

    /**
     * @brief Returns if the editor should return to the launcher
     *
     * @return true If it should return
     * @return false If application should close
     */
    [[nodiscard]] auto
    GetReturnToLauncher() const -> bool override;

    /**
     * @brief Sets if the editor should close
     *
     * @param closeSignal True if the editor should close, false if not
     */
    void
    SetCloseSignal(bool closeSignal) override;

    /**
     * @brief Returns the stores close signal
     *
     * @return true If the editor is done running
     * @return false If not
     */
    [[nodiscard]] auto
    GetCloseSignal() const -> bool override;

    /**
     * @brief Sets the rendering device info string
     *
     * @param deviceInfo To display the GPU and driver version
     */
    void
    SetDeviceInfo(const std::string& deviceInfo) override;

    /**
     * @brief Returns the stores device info string
     *
     * @return Stores device info string
     */
    [[nodiscard]] auto
    GetDeviceInfo() const -> const std::string& override;

    /**
     * @brief Set the Time Since Start object
     *
     * @param timeStamp
     */
    void
    SetInitialTimeStamp(const TimeStamp& timeStamp) override;

    /**
     * @brief Returns the elapsed time since the start of the editor
     *
     * @return Time in seconds
     */
    [[nodiscard]] auto
    GetTimeSinceStart() const -> double override;
  };
}