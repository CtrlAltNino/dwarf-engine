#pragma once

#include "Core/Base.h"
#include <boost/di.hpp>
#include "Editor/Editor.h"
#include "Launcher/IProjectLauncher.h"

namespace Dwarf
{
  class EditorInjector
  {
  public:
    static boost::di::injector<std::shared_ptr<Editor>>
    CreateInjector(SavedProject selectedProject);
  };
}