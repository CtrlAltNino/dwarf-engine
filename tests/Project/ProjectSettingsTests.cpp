#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Project/ProjectSettings.h"
#include "Logging/IDwarfLogger.h"
#include "Core/UUID.h"
#include <nlohmann/json.hpp>
#include <memory>
#include <filesystem>

using namespace Dwarf;
using namespace testing;

// Mock class for IDwarfLogger
class MockLogger : public IDwarfLogger
{
public:
  MOCK_METHOD(void, LogDebug, (const Log logMessage), (const, override));
  MOCK_METHOD(void, LogInfo, (const Log logMessage), (const, override));
  MOCK_METHOD(void, LogWarn, (const Log logMessage), (const, override));
  MOCK_METHOD(void, LogError, (const Log logMessage), (const, override));
};

// Mock class for IFileHandler
class MockFileHandler : public IFileHandler
{
public:
  MOCK_METHOD(std::filesystem::path, GetDocumentsPath, (), (override));
  MOCK_METHOD(std::filesystem::path, GetProjectSettingsPath, (), (override));
  MOCK_METHOD(bool,
              FileExists,
              (const std::filesystem::path& filePath),
              (override));
  MOCK_METHOD(std::string,
              ReadFile,
              (const std::filesystem::path& filePath),
              (override));
  MOCK_METHOD(void,
              WriteToFile,
              (const std::filesystem::path& filePath, std::string_view content),
              (override));
  MOCK_METHOD(bool,
              DirectoyExists,
              (const std::filesystem::path& path),
              (override));
  MOCK_METHOD(void,
              CreateDirectoryAt,
              (const std::filesystem::path& path),
              (override));
  MOCK_METHOD(void,
              OpenPathInFileBrowser,
              (const std::filesystem::path& path),
              (override));
  MOCK_METHOD(void, LaunchFile, (std::filesystem::path path), (override));
  MOCK_METHOD(void,
              Copy,
              (const std::filesystem::path& from,
               const std::filesystem::path& to),
              (override));
  MOCK_METHOD(void,
              Rename,
              (const std::filesystem::path& oldPath,
               const std::filesystem::path& newPath),
              (override));
  MOCK_METHOD(void, Duplicate, (const std::filesystem::path& path), (override));
  MOCK_METHOD(void, Delete, (const std::filesystem::path& path), (override));
};

class ProjectSettingsTest : public ::testing::Test
{
protected:
  std::shared_ptr<MockLogger>      mockLogger;
  std::shared_ptr<MockFileHandler> mockFileHandler;
  std::filesystem::path            testPath;

  void
  SetUp() override
  {
    mockLogger = std::make_shared<MockLogger>();
    mockFileHandler = std::make_shared<MockFileHandler>();
    testPath = ".";
  }

  void
  TearDown() override
  {
    // Clean up test files if necessary
  }
};

TEST_F(ProjectSettingsTest, Constructor_LoadsProjectSettings_FileExists)
{
  // Arrange
  std::filesystem::path settingsPath = testPath / "projectSettings.dproj";
  nlohmann::json        testProjectsettings;
  testProjectsettings["projectName"] = "TestProject";
  testProjectsettings["graphicsApi"] = GraphicsApi::OpenGL;
  testProjectsettings["lastOpenedScene"] =
    "123e4567-e89b-12d3-a456-426614174000";
  testProjectsettings["projectLastOpenedDate"] = 1627847285;

  EXPECT_CALL(*mockFileHandler, FileExists(settingsPath))
    .WillOnce(Return(true));
  EXPECT_CALL(*mockFileHandler, ReadFile(settingsPath))
    .WillOnce(Return(testProjectsettings.dump(2)));
  EXPECT_CALL(*mockLogger, LogInfo(_)).Times(1);
  EXPECT_CALL(*mockLogger, LogError(_)).Times(0);

  // Act
  auto projectSettings = std::make_unique<ProjectSettings>(
    ProjectPath{ testPath }, mockLogger, mockFileHandler);

  // Assert
  EXPECT_EQ(projectSettings->GetProjectName(), "TestProject");
  EXPECT_EQ(projectSettings->GetGraphicsApi(), GraphicsApi::OpenGL);
  EXPECT_EQ(projectSettings->GetLastOpenedScene()->ToString(),
            "123e4567-e89b-12d3-a456-426614174000");
  EXPECT_EQ(projectSettings->GetLastOpenedTimeStamp(), 1627847285);
}

TEST_F(ProjectSettingsTest, Constructor_LoadsProjectSettings_FileDoesNotExist)
{
  // Arrange
  std::filesystem::path settingsPath = testPath / "projectSettings.dproj";

  // EXPECT_CALL(*mockLogger, LogInfo(_)).Times(1);
  EXPECT_CALL(*mockLogger, LogError(_)).Times(1);

  // Act
  auto projectSettings = std::make_unique<ProjectSettings>(
    ProjectPath{ testPath }, mockLogger, mockFileHandler);

  // Assert
  EXPECT_EQ(projectSettings->GetProjectName(), "");
  EXPECT_EQ(projectSettings->GetGraphicsApi(), GraphicsApi::None);
  EXPECT_EQ(projectSettings->GetLastOpenedScene(), std::nullopt);
  EXPECT_EQ(projectSettings->GetLastOpenedTimeStamp(), 0);
}

TEST_F(ProjectSettingsTest, SaveUnchangedProjectSettings)
{
  // Writing a functioning project settings file
  std::filesystem::path settingsPath = testPath / "projectSettings.dproj";
  nlohmann::json        testProjectsettings;
  testProjectsettings["projectName"] = "TestProject";
  testProjectsettings["graphicsApi"] = GraphicsApi::OpenGL;
  testProjectsettings["lastOpenedScene"] =
    "123e4567-e89b-12d3-a456-426614174000";
  testProjectsettings["projectLastOpenedDate"] = 1627847285;

  EXPECT_CALL(*mockFileHandler, FileExists(settingsPath))
    .WillOnce(Return(true));
  EXPECT_CALL(*mockFileHandler, ReadFile(settingsPath))
    .WillOnce(Return(testProjectsettings.dump(2)));
  EXPECT_CALL(*mockFileHandler, WriteToFile(settingsPath, _)).Times(1);

  // Creating a new project settings object
  auto projectSettings = std::make_unique<ProjectSettings>(
    ProjectPath{ testPath }, mockLogger, mockFileHandler);

  // Directly saving the project settings
  projectSettings->Save();
}

TEST_F(ProjectSettingsTest, SaveChangedProjectSettings)
{
  // Writing a functioning project settings file
  std::filesystem::path settingsPath = testPath / "projectSettings.dproj";
  nlohmann::json        testProjectsettings;
  testProjectsettings["projectName"] = "TestProject";
  testProjectsettings["graphicsApi"] = GraphicsApi::OpenGL;
  testProjectsettings["lastOpenedScene"] =
    "123e4567-e89b-12d3-a456-426614174000";
  testProjectsettings["projectLastOpenedDate"] = 1627847285;

  EXPECT_CALL(*mockFileHandler, FileExists(settingsPath))
    .WillOnce(Return(true));
  EXPECT_CALL(*mockFileHandler, ReadFile(settingsPath))
    .WillOnce(Return(testProjectsettings.dump(2)));
  EXPECT_CALL(*mockFileHandler, WriteToFile(settingsPath, _)).Times(1);
  EXPECT_CALL(*mockLogger, LogInfo(_)).Times(1);

  // Creating a new project settings object
  auto projectSettings = std::make_unique<ProjectSettings>(
    ProjectPath{ testPath }, mockLogger, mockFileHandler);

  projectSettings->SetProjectName("CoolProject");
  projectSettings->SetGraphicsApi(GraphicsApi::Vulkan);
  projectSettings->SetLastOpenedScene(
    UUID("123e4567-e89b-12d3-a456-426614174001"));
  projectSettings->SetLastOpenedTimeStamp(1627847286);

  EXPECT_CALL(*mockLogger, LogInfo(_)).Times(1);

  // Act
  projectSettings->Save();

  nlohmann::json savedProjectSettings =
    nlohmann::json::parse(FileHandler::ReadFile(settingsPath));

  EXPECT_EQ(savedProjectSettings[PROJECT_NAME_KEY], "CoolProject");
  EXPECT_EQ(savedProjectSettings[GRAPHICS_API_KEY], 4);
  EXPECT_EQ(savedProjectSettings[LAST_OPENED_SCENE_KEY],
            "123e4567-e89b-12d3-a456-426614174001");
  EXPECT_EQ(savedProjectSettings[PROJECT_LAST_OPENED_DATE_KEY], 1627847286);
  FileHandler::Delete(settingsPath);
}

TEST_F(ProjectSettingsTest, SetAndGetProjectName)
{
  // Arrange
  auto projectSettings =
    std::make_unique<ProjectSettings>(ProjectPath{ testPath }, mockLogger);

  // Act
  projectSettings->SetProjectName("TestProject");

  // Assert
  EXPECT_EQ(projectSettings->GetProjectName(), "TestProject");
}

TEST_F(ProjectSettingsTest, SetAndGetGraphicsApi)
{
  // Arrange
  auto projectSettings =
    std::make_unique<ProjectSettings>(ProjectPath{ testPath }, mockLogger);

  // Act
  projectSettings->SetGraphicsApi(GraphicsApi::Vulkan);

  // Assert
  EXPECT_EQ(projectSettings->GetGraphicsApi(), GraphicsApi::Vulkan);
}

TEST_F(ProjectSettingsTest, SetAndGetLastOpenedScene)
{
  // Arrange
  auto projectSettings =
    std::make_unique<ProjectSettings>(ProjectPath{ testPath }, mockLogger);

  // Act
  UUID sceneUUID("123e4567-e89b-12d3-a456-426614174000");
  projectSettings->SetLastOpenedScene(sceneUUID);

  // Assert
  EXPECT_EQ(projectSettings->GetLastOpenedScene()->ToString(),
            "123e4567-e89b-12d3-a456-426614174000");
}

TEST_F(ProjectSettingsTest, SetAndGetLastOpenedTimeStamp)
{
  // Arrange
  auto projectSettings =
    std::make_unique<ProjectSettings>(ProjectPath{ testPath }, mockLogger);

  // Act
  time_t timeStamp = 1627847285;
  projectSettings->SetLastOpenedTimeStamp(timeStamp);

  // Assert
  EXPECT_EQ(projectSettings->GetLastOpenedTimeStamp(), timeStamp);
}