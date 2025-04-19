#pragma once

#include "IProjectNotFoundModal.hpp"
#include "Launcher/LauncherAssets/ILauncherAssets.hpp"
#include "Launcher/LauncherData/ILauncherData.hpp"
#include "Launcher/SavedProjects/ISavedProjects.hpp"

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