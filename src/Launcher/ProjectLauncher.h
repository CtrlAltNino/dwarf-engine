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
    Ref<ProjectLauncherModel> m_Model;
    Ref<ProjectLauncherView>  m_View;
    Ref<Window>               m_Window;

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