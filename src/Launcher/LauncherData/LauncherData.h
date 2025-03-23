#pragma once

#include "Launcher/LauncherData/ILauncherData.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class LauncherData : public ILauncherData
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    ProjectChooserState           mState;
    std::optional<SavedProject>   mSelectedProject;

  public:
    LauncherData(std::shared_ptr<IDwarfLogger> logger);
    ~LauncherData() override;

    /**
     * @brief Returns the current state of the launcher
     *
     * @return Enum value representing the state
     */
    [[nodiscard]] auto
    GetState() const -> ProjectChooserState override;

    /**
     * @brief Sets the state of the launcher
     *
     * @param state Enum value representing the state
     */
    void
    SetState(ProjectChooserState state) override;

    /**
     * @brief Sets the selected project (For choosing it or manipulating it)
     *
     * @param project Saved project data to be selected
     */
    void
    SetSelectedProject(std::optional<SavedProject> project) override;

    /**
     * @brief Returns the currently selected project
     *
     * @return Currently selected project
     */
    [[nodiscard]] auto
    GetSelectedProject() const -> const std::optional<SavedProject>& override;
  };
}