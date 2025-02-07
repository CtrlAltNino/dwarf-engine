#pragma once

#include "IProjectNotFoundModal.h"
#include "Launcher/IProjectLauncherData.h"
#include "Launcher/LauncherAssets/ILauncherAssets.h"
#include "Launcher/SavedProjects/ISavedProjects.h"

namespace Dwarf
{
  class ProjectNotFoundModal : public IProjectNotFoundModal
  {
  private:
    std::shared_ptr<IProjectLauncherData> m_Data;
    std::shared_ptr<ISavedProjects>       m_SavedProjects;
    std::shared_ptr<ILauncherAssets>      m_LauncherAssets;

  public:
    ProjectNotFoundModal(std::shared_ptr<IProjectLauncherData> data,
                         std::shared_ptr<ISavedProjects>       savedProjects,
                         std::shared_ptr<ILauncherAssets>      launcherAssets);

    void
    RenderProjectNotFoundModal() override;
  };
}