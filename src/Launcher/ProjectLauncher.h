#pragma once

#include "Core/Base.h"
#include "Launcher/ProjectLauncherModel.h"
#include "Launcher/ProjectLauncherView.h"

#include "Window/Window.h"

namespace Dwarf
{

  class ProjectLauncher
  {
  private:
    std::shared_ptr<ProjectLauncherModel> m_Model;
    std::shared_ptr<ProjectLauncherView>  m_View;
    std::shared_ptr<Window>               m_Window;

    static ProjectLauncher* s_Instance;

  public:
    ProjectLauncher();
    std::filesystem::path
    Run();
    static ProjectLauncher*
    Get()
    {
      return s_Instance;
    }
    Window*
    GetWindow()
    {
      return m_Window.get();
    }
  };

  ProjectLauncher*
  CreateLauncher();
}