#pragma once

#include "IButtonsView.h"
#include "Launcher/LauncherAssets/ILauncherAssets.h"
#include "Launcher/LauncherData/ILauncherData.h"

namespace Dwarf
{
  class ButtonsView : public IButtonsView
  {
  private:
    std::shared_ptr<ILauncherData>   mData;
    std::shared_ptr<ISavedProjects>  mSavedProjects;
    std::shared_ptr<ILauncherAssets> mLauncherAssets;

  public:
    ButtonsView(std::shared_ptr<ILauncherData>   data,
                std::shared_ptr<ISavedProjects>  savedProjects,
                std::shared_ptr<ILauncherAssets> launcherAssets);

    void
    RenderButtons(glm::ivec2 windowSize) override;
  };
}