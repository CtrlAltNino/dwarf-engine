#pragma once

#include "Launcher/ProjectLauncher.hpp"
#include <boost/di.hpp>

namespace Dwarf
{
  class ProjectLauncherInjector
  {
  public:
    static auto
    CreateInjector() -> boost::di::injector<std::shared_ptr<ProjectLauncher>>;
  };
}