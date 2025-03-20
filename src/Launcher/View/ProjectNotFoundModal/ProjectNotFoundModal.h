#pragma once

#include "IProjectNotFoundModal.h"
#include "Launcher/LauncherAssets/ILauncherAssets.h"
#include "Launcher/LauncherData/ILauncherData.h"
#include "Launcher/SavedProjects/ISavedProjects.h"

namespace Dwarf
{
  class ProjectNotFoundModal : public IProjectNotFoundModal
  {
  private:
    std::shared_ptr<ILauncherData>   mData;
    std::shared_ptr<ISavedProjects>  mSavedProjects;
    std::shared_ptr<ILauncherAssets> mLauncherAssets;

  public:
    ProjectNotFoundModal(std::shared_ptr<ILauncherData>   data,
                         std::shared_ptr<ISavedProjects>  savedProjects,
                         std::shared_ptr<ILauncherAssets> launcherAssets);

    void
    RenderProjectNotFoundModal() override;
  };
}