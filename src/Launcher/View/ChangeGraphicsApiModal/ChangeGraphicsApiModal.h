#pragma once

#include "IChangeGraphicsApiModal.h"
#include "Launcher/LauncherData/ILauncherData.h"
#include "Launcher/LauncherAssets/ILauncherAssets.h"

namespace Dwarf
{
  class ChangeGraphicsApiModal : public IChangeGraphicsApiModal
  {
  private:
    std::shared_ptr<ILauncherData>   m_Data;
    std::shared_ptr<ISavedProjects>  m_SavedProjects;
    std::shared_ptr<ILauncherAssets> m_LauncherAssets;

  public:
    ChangeGraphicsApiModal(std::shared_ptr<ILauncherData>   data,
                           std::shared_ptr<ISavedProjects>  savedProjects,
                           std::shared_ptr<ILauncherAssets> launcherAssets);

    void
    RenderChangeGraphicsApiModal() override;
  };
}