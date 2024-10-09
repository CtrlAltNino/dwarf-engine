#pragma once

#include "Editor/Stats/IEditorStats.h"

namespace Dwarf
{
  class EditorStats : public IEditorStats
  {
  private:
    TimeStamp   m_CurrentTimeStamp;
    TimeStamp   m_LastTimeStamp;
    std::string m_DeviceInfo = "";
    bool        m_ReturnToLauncher = false;
    bool        m_CloseSignal = false;

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
  };
}