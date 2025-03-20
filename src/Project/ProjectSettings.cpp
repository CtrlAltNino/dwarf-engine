#include "ProjectSettings.h"
#include "IProjectSettings.h"

namespace Dwarf
{
  ProjectSettings::ProjectSettings(
    ProjectPath                         path,
    std::shared_ptr<IDwarfLogger>       logger,
    std::shared_ptr<IFileHandler>       fileHandler,
    std::shared_ptr<IProjectSettingsIO> projectSettingsIO)
    : mLogger(logger)
    , mProjectPath(path.t)
    , mFileHandler(fileHandler)
    , mProjectSettingsIO(projectSettingsIO)
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

  std::string
  ProjectSettings::GetProjectName() const
  {
    return mData.ProjectName;
  }

  void
  ProjectSettings::UpdateGraphicsApi(const GraphicsApi& graphicsAPI)
  {
    mData.GraphicsApi = graphicsAPI;
  }

  const GraphicsApi&
  ProjectSettings::GetGraphicsApi() const
  {
    return mData.GraphicsApi;
  }

  void
  ProjectSettings::UpdateLastOpenedScene(const UUID& sceneGUID)
  {
    mData.LastOpenedScene = sceneGUID;
  }

  const std::optional<UUID>&
  ProjectSettings::GetLastOpenedScene() const
  {
    return mData.LastOpenedScene;
  }

  void
  ProjectSettings::UpdateSerializedView(const nlohmann::json& serializedView)
  {
    mData.SerializedView = serializedView;
  }

  nlohmann::json
  ProjectSettings::GetSerializedView() const
  {
    return mData.SerializedView;
  }
}