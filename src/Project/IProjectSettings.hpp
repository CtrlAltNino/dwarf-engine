#pragma once

#include "Core/Base.hpp"
#include "Core/UUID.hpp"
#include "Utilities/ISerializable.hpp"
#include <nlohmann/json_fwd.hpp>

#define GRAPHICS_API_KEY "graphicsApi"
#define LAST_OPENED_SCENE_KEY "lastOpenedScene"
#define PROJECT_NAME_KEY "projectName"
#define VIEW_KEY "view"

namespace Dwarf
{
  struct ProjectSettingsData : ISerializable
  {
    std::string         ProjectName;
    GraphicsApi         Api = GraphicsApi::None;
    std::optional<UUID> LastOpenedScene = std::nullopt;
    nlohmann::json      SerializedView;

    // Equality operator for ProjectInformation.
    auto
    operator==(const ProjectSettingsData& other) const -> bool
    {
      return ProjectName == other.ProjectName && Api == other.Api &&
             LastOpenedScene == other.LastOpenedScene &&
             SerializedView == other.SerializedView;
    }

    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json projectSettings;
      projectSettings[PROJECT_NAME_KEY] = ProjectName;
      projectSettings[GRAPHICS_API_KEY] = Api;
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

    /**
     * @brief Saves the project settings to disk
     *
     */
    virtual void
    Save() = 0;

    /**
     * @brief Updates the project name
     *
     * @param projectName Name of the project
     */
    virtual void
    UpdateProjectName(const std::string& projectName) = 0;

    /**
     * @brief Returns the name of the project
     *
     * @return Project name
     */
    [[nodiscard]] virtual auto
    GetProjectName() const -> std::string = 0;

    /**
     * @brief Updates the graphics api used in the project
     *
     * @param graphicsApi Graphics api to use
     */
    virtual void
    UpdateGraphicsApi(const GraphicsApi& graphicsApi) = 0;

    /**
     * @brief Returns the graphics api used in the project
     *
     * @return Enum value of the graphics api
     */
    [[nodiscard]] virtual auto
    GetGraphicsApi() const -> const GraphicsApi& = 0;

    /**
     * @brief Updates the last opened scene
     *
     * @param lastOpenedScene UUID of the scene
     */
    virtual void
    UpdateLastOpenedScene(const UUID& lastOpenedScene) = 0;

    /**
     * @brief Get the UUID of the last opened scene
     *
     * @return Returns a reference to the loaded UUID of the last opened scene
     * (Optional)
     */
    [[nodiscard]] virtual auto
    GetLastOpenedScene() const -> const std::optional<UUID>& = 0;

    /**
     * @brief Updates the serialized view
     *
     * @param serializedView The serialized view to update to
     */
    virtual void
    UpdateSerializedView(const nlohmann::json& serializedView) = 0;

    /**
     * @brief Gets the stored serialized view json
     *
     * @return The serialized json view
     */
    [[nodiscard]] virtual auto
    GetSerializedView() const -> nlohmann::json = 0;
  };
}