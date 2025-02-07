#pragma once

#include "ICreateNewProjectModal.h"
#include "Launcher/IProjectLauncherData.h"
#include "Launcher/LauncherAssets/ILauncherAssets.h"
#include "Launcher/ProjectCreator/IProjectCreator.h"

namespace Dwarf
{
  class CreateNewProjectModal : public ICreateNewProjectModal
  {
  private:
    std::shared_ptr<IProjectLauncherData> m_Data;
    std::shared_ptr<IProjectCreator>      m_ProjectCreator;
    std::shared_ptr<ILauncherAssets>      m_LauncherAssets;

  public:
    CreateNewProjectModal(std::shared_ptr<IProjectLauncherData> data,
                          std::shared_ptr<IProjectCreator>      projectCreator,
                          std::shared_ptr<ILauncherAssets>      launcherAssets);

    void
    RenderCreateNewProjectModal() override;
  };
}