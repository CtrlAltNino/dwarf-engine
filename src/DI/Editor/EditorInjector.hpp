#pragma once

#include "Editor/Editor.hpp"
#include "Launcher/SavedProjects/ISavedProjects.hpp"
#include <boost/di.hpp>

namespace Dwarf
{
  class EditorInjector
  {
  public:
    static auto
    CreateInjector(SavedProject selectedProject)
      -> boost::di::injector<std::shared_ptr<Editor>>;
  };
}