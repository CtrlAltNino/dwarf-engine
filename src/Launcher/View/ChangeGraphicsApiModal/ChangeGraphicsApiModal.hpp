#pragma once

#include "IChangeGraphicsApiModal.hpp"
#include "Launcher/LauncherAssets/ILauncherAssets.hpp"
#include "Launcher/LauncherData/ILauncherData.hpp"

namespace Dwarf
{
  class ChangeGraphicsApiModal : public IChangeGraphicsApiModal
  {
  private:
    std::shared_ptr<ILauncherData>   mData;
    std::shared_ptr<ISavedProjects>  mSavedProjects;
    std::shared_ptr<ILauncherAssets> mLauncherAssets;

  public:
    ChangeGraphicsApiModal(std::shared_ptr<ILauncherData>   data,
                           std::shared_ptr<ISavedProjects>  savedProjects,
                           std::shared_ptr<ILauncherAssets> launcherAssets);

    void
    RenderChangeGraphicsApiModal() override;
  };
}