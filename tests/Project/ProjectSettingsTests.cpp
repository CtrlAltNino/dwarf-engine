#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Project/IProjectSettingsIO.h"
#include "Project/ProjectSettings.h"
#include "Logging/IDwarfLogger.h"
#include "Core/UUID.h"
#include <nlohmann/json.hpp>
#include <memory>
#include <filesystem>
#include "Helper/TempFileHelper.h"

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
  MOCK_METHOD(std::filesystem::path, GetEngineSettingsPath, (), (override));
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
              DirectoryExists,
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
  MOCK_METHOD(std::vector<unsigned char>,
              ReadBinaryFileUnbuffered,
              (std::filesystem::path const& path),
              (override));
};

class MockProjectSettingsIO : public IProjectSettingsIO
{
public:
  MOCK_METHOD(std::optional<ProjectSettingsData>,
              LoadProjectSettings,
              (std::filesystem::path projectSettingsPath),
              (override));
  MOCK_METHOD(void,
              SaveProjectSettings,
              (ProjectSettingsData   projectSettingsData,
               std::filesystem::path projectSettingsPath),
              (override));
};

class ProjectSettingsTest : public ::testing::Test
{
protected:
  std::shared_ptr<MockLogger>            mockLogger;
  std::shared_ptr<MockFileHandler>       mockFileHandler;
  std::shared_ptr<MockProjectSettingsIO> mockProjectSettingsIO;
  std::filesystem::path                  testPath;

  void
  SetUp() override
  {
    mockLogger = std::make_shared<MockLogger>();
    mockFileHandler = std::make_shared<MockFileHandler>();
    mockProjectSettingsIO = std::make_shared<MockProjectSettingsIO>();
    testPath = ".";
  }

  void
  TearDown() override
  {
    // Clean up test files if necessary
  }
};

TEST_F(ProjectSettingsTest, SaveUnchangedProjectSettings)
{
  // Writing a functioning project settings file
  std::filesystem::path settingsPath = testPath / "projectSettings.dproj";
  ProjectPath           projectPath(settingsPath);
  ProjectSettingsData   testData;
  testData.ProjectName = "TestProject";
  testData.GraphicsApi = GraphicsApi::OpenGL;
  testData.LastOpenedScene = UUID("123e4567-e89b-12d3-a456-426614174000");
  testData.SerializedView = "";

  // EXPECT_CALL(*mockLogger, LogInfo(_)).Times(1);
  EXPECT_CALL(*mockProjectSettingsIO, LoadProjectSettings(Eq(settingsPath)))
    .Times(1);
  EXPECT_CALL(*mockProjectSettingsIO,
              SaveProjectSettings(testData, Eq(settingsPath)))
    .Times(1);
  // Creating a new project settings object
  auto projectSettings = std::make_unique<ProjectSettings>(
    projectPath, mockLogger, mockFileHandler, mockProjectSettingsIO);

  projectSettings->UpdateProjectName("TestProject");
  projectSettings->UpdateGraphicsApi(GraphicsApi::OpenGL);
  projectSettings->UpdateLastOpenedScene(
    UUID("123e4567-e89b-12d3-a456-426614174000"));
  projectSettings->UpdateSerializedView("");
  // Directly saving the project settings
  projectSettings->Save();
}