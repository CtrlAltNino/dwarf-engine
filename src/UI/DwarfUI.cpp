#include "pch.hpp"

#include "DwarfUI.hpp"

namespace Dwarf
{
  auto
  DwarfUI::InputTextCallback(ImGuiInputTextCallbackData* data) -> int
  {
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
      auto* str = static_cast<std::string*>(data->UserData);
      str->resize(data->BufTextLen);
      data->Buf = str->data();
    }
    return 0;
  }
}