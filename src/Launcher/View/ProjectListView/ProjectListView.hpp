#pragma once

#include "IProjectListView.hpp"
#include "Launcher/LauncherAssets/ILauncherAssets.hpp"
#include "Launcher/LauncherData/ILauncherData.hpp"
#include "Launcher/SavedProjects/Sorter/ISavedProjectsSorter.hpp"
#include "Utilities/FileHandler/IFileHandler.hpp"

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