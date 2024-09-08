#include "ProjectListIO.h"
#include "Utilities/FileHandler.h"

namespace Dwarf
{
  ProjectListIO::ProjectListIO(IDwarfLogger& logger)
    : m_Logger(logger)
  {
    m_Logger.LogInfo(Log("ProjectListIO created", "ProjectListIO"));
  }

  ProjectListIO::~ProjectListIO()
  {
    m_Logger.LogInfo(Log("ProjectListIO destroyed", "ProjectListIO"));
  }

  std::vector<ProjectInformation>
  ProjectListIO::LoadProjectList() const
  {
    std::vector<ProjectInformation> projectList;
    std::filesystem::path           savedProjectsPath =
      FileHandler::GetProjectSettingsPath() / "savedProjects.json";

    m_Logger.LogInfo(Log(
      fmt::format("Loading project list from {}", savedProjectsPath.string()),
      "ProjectListIO"));
    std::string fileContent = FileHandler::ReadFile(savedProjectsPath);

    if (!fileContent.empty())
    {
      nlohmann::json jsonObject = nlohmann::json::parse(fileContent);

      m_Logger.LogInfo(Log("Parsing project list", "ProjectListIO"));

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

        m_Logger.LogInfo(
          Log(fmt::format("Loaded {} projects", projectList.size()),
              "ProjectListIO"));
      }
    }
    else
    {
      m_Logger.LogWarn(
        Log("No projects found in project list", "ProjectListIO"));
    }
    return projectList;
  }

  ProjectInformation
  ProjectListIO::LoadProject(const std::filesystem::path& path) const
  {
    std::filesystem::path projectSettingsPath = path / "projectSettings.dproj";
    std::string fileContent = FileHandler::ReadFile(projectSettingsPath);

    m_Logger.LogInfo(Log(fmt::format("Loading project information from {}",
                                     projectSettingsPath.string()),
                         "ProjectListIO"));

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

      m_Logger.LogInfo(
        Log(fmt::format("Loaded project information for {}", foundInfo.name),
            "ProjectListIO"));
    }
    else
    {
      m_Logger.LogWarn(Log(fmt::format("No project information found at {}",
                                       projectSettingsPath.string()),
                           "ProjectListIO"));
    }

    return foundInfo;
  }

  void
  ProjectListIO::SaveProjectList(
    const std::vector<ProjectInformation>& projectList) const
  {
    nlohmann::json jsonObject;
    m_Logger.LogInfo(Log("Saving project list", "ProjectListIO"));
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

    m_Logger.LogInfo(Log("Project list saved", "ProjectListIO"));
  }
}