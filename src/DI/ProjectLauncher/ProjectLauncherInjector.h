#pragma once

#include "Launcher/ProjectLauncher.h"
#include <boost/di.hpp>

namespace Dwarf
{
  class ProjectLauncherInjector
  {
  public:
    static auto
    createInjector() -> boost::di::injector<std::shared_ptr<ProjectLauncher>>;
  };
}