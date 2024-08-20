#pragma once

#include "Utilities/TimeUtilities.h"

namespace Dwarf
{
  class IEditorStats
  {
  public:
    virtual ~IEditorStats() = default;

    virtual const TimeStamp&
    GetCurrentTimeStamp() const = 0;

    virtual void
    SetCurrentTimeStamp(const TimeStamp& timeStamp) = 0;

    virtual const TimeStamp&
    GetLastTimeStamp() const = 0;

    virtual void
    SetLastTimeStamp(const TimeStamp& timeStamp) = 0;

    virtual double
    GetDeltaTime() const = 0;
  };
}