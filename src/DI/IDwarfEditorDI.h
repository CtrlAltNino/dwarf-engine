#pragma once

#include "Editor/IEditor.h"
#include <boost/di.hpp>

namespace Dwarf
{
  class IDwarfEditorDI
  {
  public:
    virtual ~IDwarfEditorDI() = default;
    virtual const boost::di::injector<IEditor>&
    Get() const = 0;
  };
}