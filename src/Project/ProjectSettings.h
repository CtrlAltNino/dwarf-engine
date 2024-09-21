#pragma once

#include "pch.h"
#include "Core/Base.h"
#include "IProjectSettings.h"
#include "Logging/IDwarfLogger.h"
#include "ProjectTypes.h"
#include <boost/di.hpp>

namespace Dwarf
{
#define GRAPHICS_API_KEY "graphicsApi"
#define LAST_OPENED_SCENE_KEY "lastOpenedScene"
#define PROJECT_LAST_OPENED_DATE_KEY "projectLastOpenedDate"
#define PROJECT_NAME_KEY "projectName"

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
    std::filesystem::path         m_ProjectSettingsPath;
    std::string                   m_Name;
    time_t                        m_LastOpenedTimeStamp;
    GraphicsApi                   m_GraphicsApi;
    std::optional<UUID>           m_LastOpenedScene;
    std::shared_ptr<IDwarfLogger> m_Logger;
    LoadStatus                    m_LoadStatus;

    void
    Load();

  public:
    BOOST_DI_INJECT(ProjectSettings,
                    ProjectPath                   path,
                    std::shared_ptr<IDwarfLogger> logger);
    ~ProjectSettings() override = default;

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
    SetLastOpenedScene(const UUID& lastOpenedScene) override;

    const std::optional<UUID>&
    GetLastOpenedScene() const override;
  };
}