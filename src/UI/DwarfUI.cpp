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

  auto
  DwarfUI::InputTextString(const char*         label,
                           std::string&        str,
                           ImGuiInputTextFlags flags) -> bool
  {
    return ImGui::InputText(label,
                            str.data(),
                            str.capacity() + 1,
                            ImGuiInputTextFlags_CallbackResize | flags,
                            DwarfUI::InputTextCallback,
                            &str);
  }
}