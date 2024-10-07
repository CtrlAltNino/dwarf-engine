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

    virtual void
    SetReturnToLauncher(bool returnToLauncher) = 0;

    virtual bool
    GetReturnToLauncher() const = 0;

    virtual void
    SetCloseSignal(bool closeSignal) = 0;

    virtual bool
    GetCloseSignal() const = 0;

    virtual void
    SetTimeSinceStart(const double& timeStamp) = 0;

    virtual const double&
    GetTimeSinceStart() const = 0;
  };
}