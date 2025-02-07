#pragma once

#include "IChangeGraphicsApiModal.h"
#include "Launcher/IProjectLauncherData.h"
#include "Launcher/LauncherAssets/ILauncherAssets.h"

namespace Dwarf
{
  class ChangeGraphicsApiModal : public IChangeGraphicsApiModal
  {
  private:
    std::shared_ptr<IProjectLauncherData> m_Data;
    std::shared_ptr<ISavedProjects>       m_SavedProjects;
    std::shared_ptr<ILauncherAssets>      m_LauncherAssets;

  public:
    ChangeGraphicsApiModal(std::shared_ptr<IProjectLauncherData> data,
                           std::shared_ptr<ISavedProjects>       savedProjects,
                           std::shared_ptr<ILauncherAssets> launcherAssets);

    void
    RenderChangeGraphicsApiModal() override;
  };
}