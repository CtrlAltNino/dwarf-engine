#pragma once

#include "Editor/Editor.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
#include <boost/di.hpp>

namespace Dwarf
{
  class EditorInjector
  {
  public:
    static auto
    createInjector(SavedProject selectedProject)
      -> boost::di::injector<std::shared_ptr<Editor>>;
  };
}