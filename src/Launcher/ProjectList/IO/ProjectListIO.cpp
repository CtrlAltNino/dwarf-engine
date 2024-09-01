#include "ProjectListIO.h"
#include "Utilities/FileHandler.h"

namespace Dwarf
{
  std::vector<ProjectInformation>
  ProjectListIO::LoadProjectList() const
  {
    std::vector<ProjectInformation> projectList;
    bool                            success = true;
    std::filesystem::path           savedProjectsPath =
      FileHandler::GetProjectSettingsPath() / "savedProjects.json";
    std::string fileContent = FileHandler::ReadFile(savedProjectsPath);
    if (!fileContent.empty())
    {
      nlohmann::json jsonObject = nlohmann::json::parse(fileContent);

      if (jsonObject.contains("projects"))
      {
        for (int i = 0; i < jsonObject["projects"].size(); i++)
        {
          ProjectInformation projectToAdd;
          if (jsonObject["projects"][i].contains("name"))
          {
            projectToAdd.name = jsonObject["projects"][i]["name"];
          }

          if (jsonObject["projects"][i].contains("path"))
          {
            projectToAdd.path = std::filesystem::path(
              jsonObject["projects"][i]["path"].get<std::filesystem::path>());
          }

          if (jsonObject["projects"][i].contains("lastOpened"))
          {
            projectToAdd.lastOpened = jsonObject["projects"][i]["lastOpened"];
          }

          if (jsonObject["projects"][i].contains("graphicsApi"))
          {
            projectToAdd.graphicsApi = jsonObject["projects"][i]["graphicsApi"];
          }

          projectList.push_back({ projectToAdd });
        }
      }
    }
    return projectList;
  }

  ProjectInformation
  ProjectListIO::LoadProject(const std::filesystem::path& path) const
  {
    std::filesystem::path projectSettingsPath = path / "projectSettings.dproj";
    std::string fileContent = FileHandler::ReadFile(projectSettingsPath);

    ProjectInformation foundInfo;

    if (!fileContent.empty())
    {
      nlohmann::json jsonObject = nlohmann::json::parse(fileContent);

      if (jsonObject.contains("projectInformation"))
      {
        foundInfo.name = jsonObject["projectInformation"]["projectName"];
        foundInfo.path = path;
        foundInfo.graphicsApi = jsonObject["projectInformation"]["graphicsApi"];
        foundInfo.lastOpened =
          jsonObject.contains("lastOpened")
            ? (int)jsonObject["projectInformation"]["lastOpened"]
            : -1;
      }
    }

    return foundInfo;
  }

  void
  ProjectListIO::SaveProjectList(
    const std::vector<ProjectInformation>& projectList) const
  {
    nlohmann::json jsonObject;
    for (int i = 0; i < projectList.size(); i++)
    {
      jsonObject["projects"][i]["name"] = projectList[i].name;
      jsonObject["projects"][i]["path"] = projectList[i].path.t.string();
      jsonObject["projects"][i]["lastOpened"] = projectList[i].lastOpened;
      jsonObject["projects"][i]["graphicsApi"] = projectList[i].graphicsApi;
    }

    std::string           fileContent = jsonObject.dump(4);
    std::filesystem::path settingsPath = FileHandler::GetProjectSettingsPath();
    if (!FileHandler::DirectoyExists(settingsPath))
    {
      FileHandler::CreateDirectoryAt(settingsPath);
    }
    std::filesystem::path savedProjectsPath =
      settingsPath / "savedProjects.json";
    FileHandler::WriteToFile(savedProjectsPath.make_preferred(), fileContent);
  }
}