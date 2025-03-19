#pragma once

#include "Core/Base.h"
#include "Core/UUID.h"
#include "Editor/Modules/IGuiModule.h"
#include "Utilities/ISerializable.h"
#include "pch.h"
#include <nlohmann/json_fwd.hpp>


#define GRAPHICS_API_KEY "graphicsApi"
#define LAST_OPENED_SCENE_KEY "lastOpenedScene"
#define PROJECT_NAME_KEY "projectName"
#define VIEW_KEY "view"

namespace Dwarf
{
  struct ProjectSettingsData : ISerializable
  {
    std::string         ProjectName = "";
    GraphicsApi         GraphicsApi = GraphicsApi::None;
    std::optional<UUID> LastOpenedScene = std::nullopt;
    nlohmann::json      SerializedView;

    // Equality operator for ProjectInformation.
    bool
    operator==(const ProjectSettingsData& other) const
    {
      return ProjectName == other.ProjectName &&
             GraphicsApi == other.GraphicsApi &&
             LastOpenedScene == other.LastOpenedScene &&
             SerializedView == other.SerializedView;
    }

    nlohmann::json
    Serialize()
    {
      nlohmann::json projectSettings;
      projectSettings[PROJECT_NAME_KEY] = ProjectName;
      projectSettings[GRAPHICS_API_KEY] = GraphicsApi;
      projectSettings[LAST_OPENED_SCENE_KEY] =
        LastOpenedScene.has_value() ? LastOpenedScene->toString() : "";
      projectSettings[VIEW_KEY] = SerializedView;

      return projectSettings;
    }
  };

  class IProjectSettings
  {
  public:
    virtual ~IProjectSettings() = default;

    virtual void
    Save() = 0;

    virtual void
    UpdateProjectName(const std::string& projectName) = 0;

    virtual std::string
    GetProjectName() const = 0;

    virtual void
    UpdateGraphicsApi(const GraphicsApi& graphicsApi) = 0;

    virtual const GraphicsApi&
    GetGraphicsApi() const = 0;

    virtual void
    UpdateLastOpenedScene(const UUID& lastOpenedScene) = 0;

    virtual const std::optional<UUID>&
    GetLastOpenedScene() const = 0;

    virtual void
    UpdateSerializedView(const nlohmann::json& serializedView) = 0;

    virtual nlohmann::json
    GetSerializedView() const = 0;
  };
}