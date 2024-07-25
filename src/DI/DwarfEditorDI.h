#pragma once

#include "Editor/Editor.h"
#include <boost/di.hpp>

namespace Dwarf
{
  class DwarfEditorDI
  {
  public:
    static boost::di::injector<Editor>
    CreateInjector(std::filesystem::path projectPath);
  };
}