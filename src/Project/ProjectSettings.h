#pragma once

#include "pch.h"
#include "IProjectSettings.h"
#include "Logging/IDwarfLogger.h"
#include <boost/serialization/strong_typedef.hpp>

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
    BOOST_STRONG_TYPEDEF(std::filesystem::path, ProjectPath);

    struct ProjectSettingsData
    {
      ProjectPath                Path;
      std::string                Name;
      time_t                     LastOpenedTimeStamp;
      GraphicsApi                GraphicsApi;
      AssetReference<SceneAsset> LastOpenedScene;
    };

  private:
    std::shared_ptr<IDwarfLogger> m_Logger;
    ProjectSettingsData           m_Data;
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
    SetLastOpenedScene(
      const AssetReference<SceneAsset>& lastOpenedScene) override;

    const AssetReference<SceneAsset>&
    GetLastOpenedScene() const override;
  };
}