#include "EditorStats.h"
#include "Utilities/TimeUtilities.h"
#include "pch.h"

namespace Dwarf
{
  EditorStats::EditorStats(std::shared_ptr<IDwarfLogger> logger)
    : mLogger(std::move(logger))
  {
    mLogger->LogDebug(Log("EditorStats created.", "EditorStats"));
  }

  EditorStats::~EditorStats()
  {
    mLogger->LogDebug(Log("EditorStats destroyed.", "EditorStats"));
  }

  auto
  EditorStats::GetCurrentTimeStamp() const -> const TimeStamp&
  {
    return mCurrentTimeStamp;
  }

  void
  EditorStats::SetCurrentTimeStamp(const TimeStamp& timeStamp)
  {
    mCurrentTimeStamp = timeStamp;
  }

  auto
  EditorStats::GetLastTimeStamp() const -> const TimeStamp&
  {
    return mLastTimeStamp;
  }

  void
  EditorStats::SetLastTimeStamp(const TimeStamp& timeStamp)
  {
    mLastTimeStamp = timeStamp;
  }

  auto
  EditorStats::GetDeltaTime() const -> double
  {
    return TimeUtilities::GetDifferenceInSeconds(mCurrentTimeStamp,
                                                 mLastTimeStamp);
  }

  void
  EditorStats::SetReturnToLauncher(bool returnToLauncher)
  {
    mReturnToLauncher = returnToLauncher;
  }

  auto
  EditorStats::GetReturnToLauncher() const -> bool
  {
    return mReturnToLauncher;
  }

  void
  EditorStats::SetCloseSignal(bool closeSignal)
  {
    mCloseSignal = closeSignal;
  }

  auto
  EditorStats::GetCloseSignal() const -> bool
  {
    return mCloseSignal;
  }

  void
  EditorStats::SetDeviceInfo(const std::string& deviceInfo)
  {
    mDeviceInfo = deviceInfo;
  }

  auto
  EditorStats::GetDeviceInfo() const -> const std::string&
  {
    return mDeviceInfo;
  }

  void
  EditorStats::SetInitialTimeStamp(const TimeStamp& timeStamp)
  {
    mInitialTimeStamp = timeStamp;
  }

  auto
  EditorStats::GetTimeSinceStart() const -> double
  {
    return TimeUtilities::GetDifferenceInSeconds(mCurrentTimeStamp,
                                                 mInitialTimeStamp);
  }
}