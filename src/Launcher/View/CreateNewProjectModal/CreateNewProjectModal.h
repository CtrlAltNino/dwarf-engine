#pragma once

#include "ICreateNewProjectModal.h"
#include "Launcher/LauncherAssets/ILauncherAssets.h"
#include "Launcher/LauncherData/ILauncherData.h"
#include "Launcher/ProjectCreator/IProjectCreator.h"

namespace Dwarf
{
  class CreateNewProjectModal : public ICreateNewProjectModal
  {
  private:
    std::shared_ptr<ILauncherData>   mData;
    std::shared_ptr<IProjectCreator> mProjectCreator;
    std::shared_ptr<ILauncherAssets> mLauncherAssets;

  public:
    CreateNewProjectModal(std::shared_ptr<ILauncherData>   data,
                          std::shared_ptr<IProjectCreator> projectCreator,
                          std::shared_ptr<ILauncherAssets> launcherAssets);

    void
    RenderCreateNewProjectModal() override;
  };
}