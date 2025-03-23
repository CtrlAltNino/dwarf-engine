#include "Launcher/SavedProjects/IO/SavedProjectsIO.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Logging/IDwarfLogger.h"
#include "Project/ProjectSettingsIO.h"
#include "Utilities/FileHandler/IFileHandler.h"
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

Matcher<std::string_view>
JsonStringEquivalent(const std::string_view expected_json_str)
{
  return Truly(
    [expected_json_str](const std::string_view actual_json_str)
    {
      nlohmann::json expected = nlohmann::json::parse(expected_json_str);
      nlohmann::json actual = nlohmann::json::parse(actual_json_str);
      return expected == actual; // Compare parsed JSON objects
    });
}

class SavedProjectsIOTest : public ::testing::Test
{
protected:
  std::shared_ptr<MockLogger>      mockLogger;
  std::shared_ptr<MockFileHandler> mockFileHandler;

  void
  SetUp() override
  {
    mockLogger = std::make_shared<MockLogger>();
    mockFileHandler = std::make_shared<MockFileHandler>();
  }

  void
  TearDown() override
  {
    // Clean up test files if necessary
  }
};

// For when there is not yet a file for saved projects
TEST_F(SavedProjectsIOTest, FileDoesNotExist)
{
  // Writing a functioning project settings file

  EXPECT_CALL(*mockLogger, LogInfo(_)).Times(2);
  EXPECT_CALL(*mockLogger, LogDebug(_)).Times(2);
  EXPECT_CALL(*mockFileHandler, GetEngineSettingsPath()).Times(1);
  EXPECT_CALL(*mockFileHandler, FileExists(_)).WillOnce(Return(false));

  // Creating a new project settings object
  auto savedProjectsIO =
    std::make_unique<SavedProjectsIO>(mockLogger, mockFileHandler);

  // Directly saving the project settings
  savedProjectsIO->LoadSavedProjects();
}

TEST_F(SavedProjectsIOTest, SaveProjectSettings)
{
  std::vector<SavedProject> testList;
  testList.push_back(SavedProject(
    GraphicsApi::OpenGL, 17923123, "TestProject", std::filesystem::path("./")));

  std::string testSerializedData = R"(
{
    "projects": [
        {
            "graphicsApi": 3,
            "lastOpened": 17923123,
            "name": "TestProject",
            "path": "./"
        }
    ]
}
)";

  EXPECT_CALL(*mockLogger, LogDebug(_)).Times(2);
  EXPECT_CALL(*mockLogger, LogInfo(_)).Times(2);
  EXPECT_CALL(*mockFileHandler, GetEngineSettingsPath()).Times(1);
  EXPECT_CALL(*mockFileHandler, DirectoryExists(_)).WillOnce(Return(true));
  EXPECT_CALL(*mockFileHandler,
              WriteToFile(_, JsonStringEquivalent(testSerializedData)))
    .Times(1);

  auto savedProjectsIO =
    std::make_unique<SavedProjectsIO>(mockLogger, mockFileHandler);

  // Directly saving the project settings
  savedProjectsIO->SaveSavedProjects(testList);
}

TEST_F(SavedProjectsIOTest, LoadProjectSettings)
{
  std::vector<SavedProject> testList;
  testList.push_back(SavedProject(
    GraphicsApi::OpenGL, 17923123, "TestProject", std::filesystem::path("./")));

  std::string testSerializedData = R"(
{
    "projects": [
        {
            "graphicsApi": 3,
            "lastOpened": 17923123,
            "name": "TestProject",
            "path": "./"
        }
    ]
}
)";

  EXPECT_CALL(*mockLogger, LogDebug(_)).Times(3);
  EXPECT_CALL(*mockLogger, LogInfo(_)).Times(2);
  EXPECT_CALL(*mockFileHandler, GetEngineSettingsPath()).Times(1);
  EXPECT_CALL(*mockFileHandler, FileExists(_)).WillOnce(Return(true));
  EXPECT_CALL(*mockFileHandler, ReadFile(_))
    .WillOnce(Return(testSerializedData));

  auto savedProjectsIO =
    std::make_unique<SavedProjectsIO>(mockLogger, mockFileHandler);

  // Directly saving the project settings
  std::vector<SavedProject> savedProjects =
    savedProjectsIO->LoadSavedProjects();

  ASSERT_EQ(testList.size(), savedProjects.size());
  for (int i = 0; i < testList.size(); i++)
  {
    ASSERT_EQ(testList[i], savedProjects[i]);
  }
}
