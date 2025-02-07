#pragma once

#include "IProjectListView.h"
#include "Launcher/IProjectLauncherData.h"
#include "Launcher/LauncherAssets/ILauncherAssets.h"
#include "Launcher/SavedProjects/Sorter/ISavedProjectsSorter.h"
#include "Utilities/FileHandler/IFileHandler.h"

namespace Dwarf
{
  class ProjectListView : public IProjectListView
  {
  private:
    std::shared_ptr<IProjectLauncherData> m_Data;
    std::shared_ptr<ISavedProjects>       m_SavedProjects;
    std::shared_ptr<ISavedProjectsSorter> m_SavedProjectsSorter;
    std::shared_ptr<IFileHandler>         m_FileHandler;
    std::shared_ptr<ILauncherAssets>      m_LauncherAssets;

  public:
    ProjectListView(std::shared_ptr<IProjectLauncherData> data,
                    std::shared_ptr<ISavedProjects>       savedProjects,
                    std::shared_ptr<ISavedProjectsSorter> savedProjectsSorter,
                    std::shared_ptr<IFileHandler>         fileHandler,
                    std::shared_ptr<ILauncherAssets>      launcherAssets);

    void
    RenderProjectList(glm::ivec2 windowSize) override;
  };
}