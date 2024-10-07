#include "Utilities/TimeUtilities.h"
#include "pch.h"
#include "EditorStats.h"

namespace Dwarf
{
  const TimeStamp&
  EditorStats::GetCurrentTimeStamp() const
  {
    return m_CurrentTimeStamp;
  }

  void
  EditorStats::SetCurrentTimeStamp(const TimeStamp& timeStamp)
  {
    m_CurrentTimeStamp = timeStamp;
  }

  const TimeStamp&
  EditorStats::GetLastTimeStamp() const
  {
    return m_LastTimeStamp;
  }

  void
  EditorStats::SetLastTimeStamp(const TimeStamp& timeStamp)
  {
    m_LastTimeStamp = timeStamp;
  }

  double
  EditorStats::GetDeltaTime() const
  {
    return TimeUtilities::GetDifferenceInSeconds(m_CurrentTimeStamp,
                                                 m_LastTimeStamp);
  }

  void
  EditorStats::SetReturnToLauncher(bool returnToLauncher)
  {
    m_ReturnToLauncher = returnToLauncher;
  }

  bool
  EditorStats::GetReturnToLauncher() const
  {
    return m_ReturnToLauncher;
  }

  void
  EditorStats::SetCloseSignal(bool closeSignal)
  {
    m_CloseSignal = closeSignal;
  }

  bool
  EditorStats::GetCloseSignal() const
  {
    return m_CloseSignal;
  }

  void
  EditorStats::SetTimeSinceStart(const double& timeStamp)
  {
    m_TimeSinceStart = timeStamp;
  }

  const double&
  EditorStats::GetTimeSinceStart() const
  {
    return m_TimeSinceStart;
  }
}