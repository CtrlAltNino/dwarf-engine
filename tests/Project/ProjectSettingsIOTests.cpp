#include "Core/UUID.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "Project/IProjectSettingsIO.hpp"
#include "Project/ProjectSettingsIO.hpp"
#include "Project/ProjectTypes.hpp"
#include "Utilities/FileHandler/IFileHandler.hpp"
#include <filesystem>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <nlohmann/json.hpp>

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
  MOCK_METHOD(std::filesystem::path, GetDocumentsPath, (), (const, override));
  MOCK_METHOD(std::filesystem::path,
              GetEngineSettingsPath,
              (),
              (const, override));
  MOCK_METHOD(bool,
              FileExists,
              (const std::filesystem::path& filePath),
              (const, override));
  MOCK_METHOD(std::string,
              ReadFile,
              (const std::filesystem::path& filePath),
              (const, override));
  MOCK_METHOD(void,
              WriteToFile,
              (const std::filesystem::path& filePath, std::string_view content),
              (const, override));
  MOCK_METHOD(bool,
              DirectoryExists,
              (const std::filesystem::path& path),
              (const, override));
  MOCK_METHOD(void,
              CreateDirectoryAt,
              (const std::filesystem::path& path),
              (const, override));
  MOCK_METHOD(void,
              OpenPathInFileBrowser,
              (const std::filesystem::path& path),
              (const, override));
  MOCK_METHOD(void,
              LaunchFile,
              (std::filesystem::path path),
              (const, override));
  MOCK_METHOD(void,
              Copy,
              (const std::filesystem::path& from,
               const std::filesystem::path& to),
              (const, override));
  MOCK_METHOD(void,
              Rename,
              (const std::filesystem::path& oldPath,
               const std::filesystem::path& newPath),
              (const, override));
  MOCK_METHOD(void,
              Duplicate,
              (const std::filesystem::path& path),
              (const, override));
  MOCK_METHOD(void,
              Delete,
              (const std::filesystem::path& path),
              (const, override));
  MOCK_METHOD(std::vector<unsigned char>,
              ReadBinaryFileUnbuffered,
              (std::filesystem::path const& path),
              (const, override));
};

class ProjectSettingsIOTest : public ::testing::Test
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

TEST_F(ProjectSettingsIOTest, FileDoesNotExist)
{
  // Writing a functioning project settings file
  std::filesystem::path settingsPath = testPath / "projectSettings.dproj";

  EXPECT_CALL(*mockLogger, LogInfo(_)).Times(1);
  EXPECT_CALL(*mockLogger, LogError(_)).Times(1);
  EXPECT_CALL(*mockFileHandler, FileExists(settingsPath)).Times(1);
  // Creating a new project settings object
  auto projectSettingsIO =
    std::make_unique<ProjectSettingsIO>(mockLogger, mockFileHandler);

  // Directly saving the project settings
  projectSettingsIO->LoadProjectSettings(testPath);
}

TEST_F(ProjectSettingsIOTest, SaveProjectSettings)
{
  // Writing a functioning project settings file
  std::filesystem::path settingsPath = testPath / "projectSettings.dproj";
  ProjectSettingsData   testData;
  testData.ProjectName = "TestProject";
  testData.GraphicsApi = GraphicsApi::OpenGL;
  testData.LastOpenedScene = UUID("123e4567-e89b-12d3-a456-426614174000");
  testData.SerializedView = "";

  std::string serializedProjectSettings = testData.Serialize().dump(2);

  // EXPECT_CALL(*mockLogger, LogInfo(_)).Times(1);
  EXPECT_CALL(*mockFileHandler,
              WriteToFile(settingsPath, Eq(serializedProjectSettings)))
    .Times(1);
  // Creating a new project settings object
  auto projectSettingsIO =
    std::make_unique<ProjectSettingsIO>(mockLogger, mockFileHandler);

  // Directly saving the project settings
  projectSettingsIO->SaveProjectSettings(testData, testPath);
}

TEST_F(ProjectSettingsIOTest, LoadProjectSettings)
{
  // Writing a functioning project settings file
  std::filesystem::path settingsPath = testPath / "projectSettings.dproj";
  ProjectSettingsData   testData;
  testData.ProjectName = "TestProject";
  testData.GraphicsApi = GraphicsApi::OpenGL;
  testData.LastOpenedScene = UUID("123e4567-e89b-12d3-a456-426614174000");
  testData.SerializedView = "";

  std::string serializedProjectSettings = testData.Serialize().dump(2);

  EXPECT_CALL(*mockLogger, LogInfo(_)).Times(1);
  EXPECT_CALL(*mockFileHandler, FileExists(settingsPath))
    .WillOnce(Return(true));
  EXPECT_CALL(*mockFileHandler, ReadFile(settingsPath))
    .WillOnce(Return(serializedProjectSettings));
  // Creating a new project settings object
  auto projectSettingsIO =
    std::make_unique<ProjectSettingsIO>(mockLogger, mockFileHandler);

  // Directly saving the project settings
  auto projectSettings = projectSettingsIO->LoadProjectSettings(testPath);

  EXPECT_TRUE(projectSettings);

  EXPECT_EQ(projectSettings, testData);
}
