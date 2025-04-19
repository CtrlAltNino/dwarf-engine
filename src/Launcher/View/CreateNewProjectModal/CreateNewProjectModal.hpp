#pragma once

#include "ICreateNewProjectModal.hpp"
#include "Launcher/LauncherAssets/ILauncherAssets.hpp"
#include "Launcher/LauncherData/ILauncherData.hpp"
#include "Launcher/ProjectCreator/IProjectCreator.hpp"

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