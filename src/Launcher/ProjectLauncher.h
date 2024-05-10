#pragma once

#include "Core/Base.h"
#include "Launcher/IProjectLauncherModel.h"
#include "Launcher/IProjectLauncherView.h"

#include "Window/Window.h"

#include <filesystem>

namespace Dwarf
{
  class ProjectLauncher
  {
  private:
    std::unique_ptr<IProjectLauncherModel> m_Model;
    std::unique_ptr<IProjectLauncherView>  m_View;
    std::unique_ptr<Window>                m_Window;

  public:
    ProjectLauncher();

    void
    Run(std::filesystem::path& projectPath);
  };
}