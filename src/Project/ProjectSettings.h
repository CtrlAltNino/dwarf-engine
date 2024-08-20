#pragma once

#include "pch.h"
#include "Core/Base.h"
#include "IProjectSettings.h"
#include "Logging/IDwarfLogger.h"
#include "ProjectTypes.h"

namespace Dwarf
{
  class LoadStatus
  {
  public:
    bool                     success;
    std::vector<std::string> missingFields;

    LoadStatus()
      : success(true)
    {
    }

    void
    addMissingField(const std::string& field)
    {
      success = false;
      missingFields.push_back(field);
    }

    bool
    isComplete() const
    {
      return success;
    }
  };

  class ProjectSettings : public IProjectSettings
  {
  private:
    // Data
    ProjectPath                   m_Path;
    std::string                   m_Name;
    time_t                        m_LastOpenedTimeStamp;
    GraphicsApi                   m_GraphicsApi;
    std::shared_ptr<UUID>         m_LastOpenedScene;
    std::shared_ptr<IDwarfLogger> m_Logger;
    LoadStatus                    m_LoadStatus;

  public:
    ProjectSettings(ProjectPath                          path,
                    const std::shared_ptr<IDwarfLogger>& logger);

    void
    Load();

    void
    Save() override;

    void
    SetProjectName(const std::string& projectName) override;

    std::string
    GetProjectName() const override;

    void
    SetLastOpenedTimeStamp(const time_t& projectLastOpenedDate) override;

    const time_t&
    GetLastOpenedTimeStamp() const override;

    void
    SetGraphicsApi(const GraphicsApi& graphicsApi) override;

    const GraphicsApi&
    GetGraphicsApi() const override;

    void
    SetLastOpenedScene(const std::shared_ptr<UUID>& lastOpenedScene) override;

    const std::shared_ptr<UUID>&
    GetLastOpenedScene() const override;
  };
}