#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Project/ProjectSettings.h"
#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler.h"
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

class ProjectSettingsTest : public ::testing::Test
{
protected:
  std::shared_ptr<MockLogger> mockLogger;
  std::filesystem::path       testPath;

  void
  SetUp() override
  {
    mockLogger = std::make_shared<MockLogger>();
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
  FileHandler::WriteToFile(settingsPath, testProjectsettings.dump(2));

  EXPECT_CALL(*mockLogger, LogInfo(_)).Times(1);
  EXPECT_CALL(*mockLogger, LogError(_)).Times(0);

  // Act
  auto projectSettings =
    std::make_unique<ProjectSettings>(ProjectPath{ testPath }, mockLogger);

  // Assert
  EXPECT_EQ(projectSettings->GetProjectName(), "TestProject");
  EXPECT_EQ(projectSettings->GetGraphicsApi(), GraphicsApi::OpenGL);
  EXPECT_EQ(projectSettings->GetLastOpenedScene()->ToString(),
            "123e4567-e89b-12d3-a456-426614174000");
  EXPECT_EQ(projectSettings->GetLastOpenedTimeStamp(), 1627847285);
  FileHandler::Delete(settingsPath);
}

TEST_F(ProjectSettingsTest, Constructor_LoadsProjectSettings_FileDoesNotExist)
{
  // Arrange
  std::filesystem::path settingsPath = testPath / "projectSettings.dproj";

  EXPECT_CALL(*mockLogger, LogInfo(_)).Times(1);
  EXPECT_CALL(*mockLogger, LogError(_)).Times(1);

  // Act
  auto projectSettings =
    std::make_unique<ProjectSettings>(ProjectPath{ testPath }, mockLogger);

  // Assert
  EXPECT_EQ(projectSettings->GetProjectName(), "");
  EXPECT_EQ(projectSettings->GetGraphicsApi(), GraphicsApi::None);
  EXPECT_EQ(projectSettings->GetLastOpenedScene(), std::nullopt);
  EXPECT_EQ(projectSettings->GetLastOpenedTimeStamp(), 0);
}

TEST_F(ProjectSettingsTest, SaveProjectSettings)
{
  // Arrange
  auto projectSettings =
    std::make_unique<ProjectSettings>(ProjectPath{ testPath }, mockLogger);
  projectSettings->SetProjectName("TestProject");
  projectSettings->SetGraphicsApi(GraphicsApi::Vulkan);
  projectSettings->SetLastOpenedScene(
    UUID("123e4567-e89b-12d3-a456-426614174000"));
  projectSettings->SetLastOpenedTimeStamp(1627847285);

  EXPECT_CALL(*mockLogger, LogInfo(_)).Times(1);

  // Act
  projectSettings->Save();

  // Assert
  std::filesystem::path settingsPath = testPath / "projectSettings.dproj";
  std::string           settingsContent = FileHandler::ReadFile(settingsPath);
  nlohmann::json projectSettingsJson = nlohmann::json::parse(settingsContent);

  EXPECT_EQ(projectSettingsJson[PROJECT_NAME_KEY], "TestProject");
  EXPECT_EQ(projectSettingsJson[GRAPHICS_API_KEY], 1);
  EXPECT_EQ(projectSettingsJson[LAST_OPENED_SCENE_KEY],
            "123e4567-e89b-12d3-a456-426614174000");
  EXPECT_EQ(projectSettingsJson[PROJECT_LAST_OPENED_DATE_KEY], 1627847285);
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