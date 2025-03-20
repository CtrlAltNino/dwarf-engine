#pragma once

#include "IProjectListView.h"
#include "Launcher/LauncherAssets/ILauncherAssets.h"
#include "Launcher/LauncherData/ILauncherData.h"
#include "Launcher/SavedProjects/Sorter/ISavedProjectsSorter.h"
#include "Utilities/FileHandler/IFileHandler.h"

namespace Dwarf
{
  class ProjectListView : public IProjectListView
  {
  private:
    std::shared_ptr<ILauncherData>        mData;
    std::shared_ptr<ISavedProjects>       mSavedProjects;
    std::shared_ptr<ISavedProjectsSorter> mSavedProjectsSorter;
    std::shared_ptr<IFileHandler>         mFileHandler;
    std::shared_ptr<ILauncherAssets>      mLauncherAssets;

  public:
    ProjectListView(std::shared_ptr<ILauncherData>        data,
                    std::shared_ptr<ISavedProjects>       savedProjects,
                    std::shared_ptr<ISavedProjectsSorter> savedProjectsSorter,
                    std::shared_ptr<IFileHandler>         fileHandler,
                    std::shared_ptr<ILauncherAssets>      launcherAssets);

    void
    RenderProjectList(glm::ivec2 windowSize) override;
  };
}