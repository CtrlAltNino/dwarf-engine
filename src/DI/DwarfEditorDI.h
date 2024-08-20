#pragma once

#include "DI/IDwarfEditorDI.h"
#include "Editor/Editor.h"
#include <boost/di.hpp>

namespace Dwarf
{
  class DwarfEditorDI : public IDwarfEditorDI
  {
  private:
    boost::di::injector<IEditor> m_Injector;

  public:
    DwarfEditorDI(std::filesystem::path projectPath);

    const boost::di::injector<IEditor>&
    Get() const override;
  };
}