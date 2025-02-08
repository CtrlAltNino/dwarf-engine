#pragma once

#include "IButtonsView.h"
#include "Launcher/LauncherData/ILauncherData.h"
#include "Launcher/LauncherAssets/ILauncherAssets.h"

namespace Dwarf
{
  class ButtonsView : public IButtonsView
  {
  private:
    std::shared_ptr<ILauncherData>   m_Data;
    std::shared_ptr<ISavedProjects>  m_SavedProjects;
    std::shared_ptr<ILauncherAssets> m_LauncherAssets;

  public:
    ButtonsView(std::shared_ptr<ILauncherData>   data,
                std::shared_ptr<ISavedProjects>  savedProjects,
                std::shared_ptr<ILauncherAssets> launcherAssets);

    void
    RenderButtons(glm::ivec2 windowSize) override;
  };
}