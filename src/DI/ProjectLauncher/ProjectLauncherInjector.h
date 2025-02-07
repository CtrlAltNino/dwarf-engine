#pragma once

#include <boost/di.hpp>
#include "Launcher/ProjectLauncher.h"

namespace Dwarf
{
  class ProjectLauncherInjector
  {
  public:
    static boost::di::injector<std::shared_ptr<ProjectLauncher>>
    CreateInjector();
  };
}