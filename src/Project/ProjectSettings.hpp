#pragma once

#include "Core/Base.hpp"
#include "IProjectSettings.hpp"
#include "IProjectSettingsIO.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "ProjectTypes.hpp"
#include "Utilities/FileHandler/IFileHandler.hpp"
#include <boost/di.hpp>

namespace Dwarf
{
  class LoadStatus
  {
  public:
    bool                     success = true;
    std::vector<std::string> missingFields;

    LoadStatus() = default;

    void
    addMissingField(const std::string& field)
    {
      success = false;
      missingFields.push_back(field);
    }

    [[nodiscard]] auto
    isComplete() const -> bool
    {
      return success;
    }
  };

  class ProjectSettings : public IProjectSettings
  {
  private:
    // Data
    std::shared_ptr<IDwarfLogger>       mLogger;
    std::shared_ptr<IFileHandler>       mFileHandler;
    std::shared_ptr<IProjectSettingsIO> mProjectSettingsIO;
    LoadStatus                          mLoadStatus;
    std::filesystem::path               mProjectPath;
    ProjectSettingsData                 mData;

  public:
    BOOST_DI_INJECT(ProjectSettings,
                    const ProjectPath&                  path,
                    std::shared_ptr<IDwarfLogger>       logger,
                    std::shared_ptr<IFileHandler>       fileHandler,
                    std::shared_ptr<IProjectSettingsIO> projectSettingsIO);
    ~ProjectSettings() override = default;

    /**
     * @brief Saves the project settings to disk
     *
     */
    void
    Save() override;

    /**
     * @brief Updates the project name
     *
     * @param projectName Name of the project
     */
    void
    UpdateProjectName(const std::string& projectName) override;

    /**
     * @brief Returns the name of the project
     *
     * @return Project name
     */
    [[nodiscard]] auto
    GetProjectName() const -> std::string override;

    /**
     * @brief Updates the graphics api used in the project
     *
     * @param graphicsApi Graphics api to use
     */
    void
    UpdateGraphicsApi(const GraphicsApi& graphicsApi) override;

    /**
     * @brief Returns the graphics api used in the project
     *
     * @return Enum value of the graphics api
     */
    [[nodiscard]] auto
    GetGraphicsApi() const -> const GraphicsApi& override;

    /**
     * @brief Updates the last opened scene
     *
     * @param lastOpenedScene UUID of the scene
     */
    void
    UpdateLastOpenedScene(const UUID& lastOpenedScene) override;

    /**
     * @brief Get the UUID of the last opened scene
     *
     * @return Returns a reference to the loaded UUID of the last opened scene
     * (Optional)
     */
    [[nodiscard]] auto
    GetLastOpenedScene() const -> const std::optional<UUID>& override;

    /**
     * @brief Updates the serialized view
     *
     * @param serializedView The serialized view to update to
     */
    void
    UpdateSerializedView(const nlohmann::json& serializedView) override;

    /**
     * @brief Gets the stored serialized view json
     *
     * @return The serialized json view
     */
    [[nodiscard]] auto
    GetSerializedView() const -> nlohmann::json override;
  };
}