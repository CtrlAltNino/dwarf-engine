#pragma once

#include "Editor/Stats/IEditorStats.h"

namespace Dwarf
{
  class EditorStats : public IEditorStats
  {
  private:
    TimeStamp m_CurrentTimeStamp;
    TimeStamp m_LastTimeStamp;
    bool      m_ReturnToLauncher = false;
    bool      m_CloseSignal = false;

  public:
    EditorStats() = default;

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
  };
}