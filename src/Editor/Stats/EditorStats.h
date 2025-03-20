#pragma once

#include "Editor/Stats/IEditorStats.h"
#include "Utilities/TimeUtilities.h"

namespace Dwarf
{
  class EditorStats : public IEditorStats
  {
  private:
    TimeStamp   mCurrentTimeStamp;
    TimeStamp   mLastTimeStamp;
    std::string mDeviceInfo = "";
    bool        mReturnToLauncher = false;
    bool        mCloseSignal = false;
    double      mTimeSinceStart = 0.0;

  public:
    EditorStats() = default;
    ~EditorStats() override = default;

    const TimeStamp&
    GetCurrentTimeStamp() const override;

    void
    SetCurrentTimeStamp(const TimeStamp& timeStamp) override;

    const TimeStamp&
    GetLastTimeStamp() const override;

    void
    SetLastTimeStamp(const TimeStamp& timeStamp) override;

    double
    GetDeltaTime() const override;

    void
    SetReturnToLauncher(bool returnToLauncher) override;

    bool
    GetReturnToLauncher() const override;

    void
    SetCloseSignal(bool closeSignal) override;

    bool
    GetCloseSignal() const override;

    void
    SetDeviceInfo(const std::string& deviceInfo) override;

    const std::string&
    GetDeviceInfo() const override;

    void
    SetTimeSinceStart(const double& timeStamp) override;

    const double&
    GetTimeSinceStart() const override;
  };
}