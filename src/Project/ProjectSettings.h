#pragma once

#include "Editor/Modules/IGuiModule.h"
#include "Utilities/FileHandler/IFileHandler.h"
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
#define VIEW_KEY "view"

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

  struct ProjectSettingsData
  {
    std::string         ProjectName;
    time_t              LastOpenedDate;
    GraphicsApi         GraphicsApi;
    std::optional<UUID> LastOpenedScene;
    nlohmann::json      SerializedView;
  };

  class ProjectSettings : public IProjectSettings
  {
  private:
    // Data
    std::shared_ptr<IDwarfLogger> m_Logger;
    std::shared_ptr<IFileHandler> m_FileHandler;
    LoadStatus                    m_LoadStatus;
    std::filesystem::path         m_ProjectSettingsPath = "";
    ProjectSettingsData           m_Data;

    void
    Load();

  public:
    BOOST_DI_INJECT(ProjectSettings,
                    ProjectPath                   path,
                    std::shared_ptr<IDwarfLogger> logger,
                    std::shared_ptr<IFileHandler> fileHandler);
    ~ProjectSettings() override = default;

    void
    Save() override;

    void
    UpdateProjectName(const std::string& projectName) override;

    std::string
    GetProjectName() const override;

    void
    UpdateLastOpenedTimeStamp(const time_t& projectLastOpenedDate) override;

    const time_t&
    GetLastOpenedTimeStamp() const override;

    void
    UpdateGraphicsApi(const GraphicsApi& graphicsApi) override;

    const GraphicsApi&
    GetGraphicsApi() const override;

    void
    UpdateLastOpenedScene(const UUID& lastOpenedScene) override;

    const std::optional<UUID>&
    GetLastOpenedScene() const override;

    void
    UpdateSerializedView(const nlohmann::json& serializedView) override;

    nlohmann::json
    GetSerializedView() const override;
  };
}