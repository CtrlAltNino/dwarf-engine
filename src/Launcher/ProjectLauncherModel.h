#pragma once

#include "Core/Base.h"
#include "IProjectLauncherModel.h"

namespace Dwarf
{
  /**
   * @brief Model for the project launcher
   */
  class ProjectLauncherModel : public IProjectLauncherModel
  {
  private:
    ///@brief Current state of the project launcher
    ProjectChooserState m_State = ProjectChooserState::Choosing;

    ///@brief ID of the selected project
    int m_SelectedProjectId = -1;

  public:
    ProjectLauncherModel();

    /**
     * @brief Get the current state of the project launcher
     * @return Current state of the project launcher
     */
    ProjectChooserState
    GetState() const override;

    /**
     * @brief Set the state of the project launcher
     * @param state New state of the project launcher
     */
    void
    SetState(ProjectChooserState state) override;

    /**
     * @brief Get the ID of the selected project
     * @return ID of the selected project
     */
    int
    GetSelectedProjectID() const override;

    /**
     * @brief Set the ID of the selected project
     * @param id ID of the selected project
     */
    void
    SetSelectedProjectID(int id) override;
  };
}