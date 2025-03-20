#pragma once

#include "IFooterView.h"
#include "Launcher/LauncherAssets/ILauncherAssets.h"
#include "Launcher/LauncherData/ILauncherData.h"

namespace Dwarf
{
  class FooterView : public IFooterView
  {
  private:
    std::shared_ptr<ILauncherData>   mData;
    std::shared_ptr<ILauncherAssets> mLauncherAssets;

  public:
    FooterView(std::shared_ptr<ILauncherData>   data,
               std::shared_ptr<ILauncherAssets> launcherAssets);

    void
    RenderFooter(glm::ivec2 windowSize) override;
  };
}