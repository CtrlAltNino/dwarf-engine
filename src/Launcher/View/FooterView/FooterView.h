#pragma once

#include "IFooterView.h"
#include "Launcher/LauncherData/ILauncherData.h"
#include "Launcher/LauncherAssets/ILauncherAssets.h"

namespace Dwarf
{
  class FooterView : public IFooterView
  {
  private:
    std::shared_ptr<ILauncherData>   m_Data;
    std::shared_ptr<ILauncherAssets> m_LauncherAssets;

  public:
    FooterView(std::shared_ptr<ILauncherData>   data,
               std::shared_ptr<ILauncherAssets> launcherAssets);

    void
    RenderFooter(glm::ivec2 windowSize) override;
  };
}