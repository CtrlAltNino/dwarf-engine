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
}