#pragma once

#include "Editor/Modules/IGuiModule.h"
#include "IProjectSettingsIO.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include "pch.h"
#include "Core/Base.h"
#include "IProjectSettings.h"
#include "Logging/IDwarfLogger.h"
#include "ProjectTypes.h"
#include <boost/di.hpp>

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
    std::shared_ptr<IDwarfLogger>       m_Logger;
    std::shared_ptr<IFileHandler>       m_FileHandler;
    std::shared_ptr<IProjectSettingsIO> m_ProjectSettingsIO;
    LoadStatus                          m_LoadStatus;
    std::filesystem::path               m_ProjectPath = "";
    ProjectSettingsData                 m_Data;

  public:
    BOOST_DI_INJECT(ProjectSettings,
                    ProjectPath                         path,
                    std::shared_ptr<IDwarfLogger>       logger,
                    std::shared_ptr<IFileHandler>       fileHandler,
                    std::shared_ptr<IProjectSettingsIO> projectSettingsIO);
    ~ProjectSettings() override = default;

    void
    Save() override;

    void
    UpdateProjectName(const std::string& projectName) override;

    std::string
    GetProjectName() const override;

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