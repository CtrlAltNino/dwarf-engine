#pragma once

#include "pch.h"
#include "Launcher/IProjectLauncherModel.h"
#include "Launcher/IProjectLauncherView.h"
#include "Window/IWindow.h"

namespace Dwarf
{
  class ProjectLauncher
  {
  private:
    std::unique_ptr<IProjectLauncherModel> m_Model;
    std::unique_ptr<IProjectLauncherView>  m_View;
    std::unique_ptr<IWindow>               m_Window;

  public:
    ProjectLauncher(std::filesystem::path& projectPath);

    void
    Run();
  };
}