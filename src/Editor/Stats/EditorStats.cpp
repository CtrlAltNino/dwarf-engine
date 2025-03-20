#include "EditorStats.h"
#include "Utilities/TimeUtilities.h"
#include "pch.h"

namespace Dwarf
{
  const TimeStamp&
  EditorStats::GetCurrentTimeStamp() const
  {
    return mCurrentTimeStamp;
  }

  void
  EditorStats::SetCurrentTimeStamp(const TimeStamp& timeStamp)
  {
    mCurrentTimeStamp = timeStamp;
  }

  const TimeStamp&
  EditorStats::GetLastTimeStamp() const
  {
    return mLastTimeStamp;
  }

  void
  EditorStats::SetLastTimeStamp(const TimeStamp& timeStamp)
  {
    mLastTimeStamp = timeStamp;
  }

  double
  EditorStats::GetDeltaTime() const
  {
    return TimeUtilities::GetDifferenceInSeconds(mCurrentTimeStamp,
                                                 mLastTimeStamp);
  }

  void
  EditorStats::SetReturnToLauncher(bool returnToLauncher)
  {
    mReturnToLauncher = returnToLauncher;
  }

  bool
  EditorStats::GetReturnToLauncher() const
  {
    return mReturnToLauncher;
  }

  void
  EditorStats::SetCloseSignal(bool closeSignal)
  {
    mCloseSignal = closeSignal;
  }

  bool
  EditorStats::GetCloseSignal() const
  {
    return mCloseSignal;
  }

  void
  EditorStats::SetDeviceInfo(const std::string& deviceInfo)
  {
    mDeviceInfo = deviceInfo;
  }

  const std::string&
  EditorStats::GetDeviceInfo() const
  {
    return mDeviceInfo;
  }

  void
  EditorStats::SetTimeSinceStart(const double& timeStamp)
  {
    mTimeSinceStart = timeStamp;
  }

  const double&
  EditorStats::GetTimeSinceStart() const
  {
    return mTimeSinceStart;
  }
}