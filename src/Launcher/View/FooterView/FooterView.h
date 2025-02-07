#pragma once

#include "IFooterView.h"
#include "Launcher/IProjectLauncherData.h"
#include "Launcher/LauncherAssets/ILauncherAssets.h"

namespace Dwarf
{
  class FooterView : public IFooterView
  {
  private:
    std::shared_ptr<IProjectLauncherData> m_Data;
    std::shared_ptr<ILauncherAssets>      m_LauncherAssets;

  public:
    FooterView(std::shared_ptr<IProjectLauncherData> data,
               std::shared_ptr<ILauncherAssets>      launcherAssets);

    void
    RenderFooter(glm::ivec2 windowSize) override;
  };
}