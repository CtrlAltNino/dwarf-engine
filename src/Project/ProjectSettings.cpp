#include "pch.hpp"

#include "IProjectSettings.hpp"
#include "ProjectSettings.hpp"

namespace Dwarf
{
  ProjectSettings::ProjectSettings(
    const ProjectPath&                  path,
    std::shared_ptr<IDwarfLogger>       logger,
    std::shared_ptr<IFileHandler>       fileHandler,
    std::shared_ptr<IProjectSettingsIO> projectSettingsIO)
    : mLogger(std::move(logger))
    , mProjectPath(path.t)
    , mFileHandler(std::move(fileHandler))
    , mProjectSettingsIO(std::move(projectSettingsIO))
  {
    std::optional<ProjectSettingsData> projectSettingsData =
      mProjectSettingsIO->LoadProjectSettings(mProjectPath);

    if (projectSettingsData)
    {
      mData = projectSettingsData.value();
    }
  }

  void
  ProjectSettings::Save()
  {
    mProjectSettingsIO->SaveProjectSettings(mData, mProjectPath);
  }

  void
  ProjectSettings::UpdateProjectName(const std::string& projectName)
  {
    mData.ProjectName = projectName;
  }

  auto
  ProjectSettings::GetProjectName() const -> std::string
  {
    return mData.ProjectName;
  }

  void
  ProjectSettings::UpdateGraphicsApi(const GraphicsApi& graphicsAPI)
  {
    mData.Api = graphicsAPI;
  }

  auto
  ProjectSettings::GetGraphicsApi() const -> const GraphicsApi&
  {
    return mData.Api;
  }

  void
  ProjectSettings::UpdateLastOpenedScene(const UUID& lastOpenedScene)
  {
    mData.LastOpenedScene = lastOpenedScene;
  }

  auto
  ProjectSettings::GetLastOpenedScene() const -> const std::optional<UUID>&
  {
    return mData.LastOpenedScene;
  }

  void
  ProjectSettings::UpdateSerializedView(const nlohmann::json& serializedView)
  {
    mData.SerializedView = serializedView;
  }

  auto
  ProjectSettings::GetSerializedView() const -> nlohmann::json
  {
    return mData.SerializedView;
  }
}