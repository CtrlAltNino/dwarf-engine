#include "Core/Asset/AssetReimporter/AssetReimporter.hpp"
#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Logging/IDwarfLogger.hpp"
#include <boost/di/extension/injections/lazy.hpp>
#include <filesystem>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace Dwarf;
using namespace testing;

class MockLogger : public IDwarfLogger
{
public:
  MOCK_METHOD(void, LogDebug, (const Log logMessage), (const, override));
  MOCK_METHOD(void, LogInfo, (const Log logMessage), (const, override));
  MOCK_METHOD(void, LogWarn, (const Log logMessage), (const, override));
  MOCK_METHOD(void, LogError, (const Log logMessage), (const, override));
};

class MockAssetDatabase : public IAssetDatabase
{
public:
  MOCK_METHOD(UUID,
              Import,
              (std::filesystem::path const& assetPath),
              (override));
  MOCK_METHOD(void, ImportDialog, (), (override));
  MOCK_METHOD(bool, Exists, (const UUID& uid), (override));
  MOCK_METHOD(bool, Exists, (const std::filesystem::path& path), (override));
  MOCK_METHOD(void, Clear, (), (override));
  MOCK_METHOD(void, Remove, (const UUID& uid), (override));
  MOCK_METHOD(void, Remove, (const std::filesystem::path& path), (override));
  MOCK_METHOD(void, ReimportAll, (), (override));
  MOCK_METHOD(void,
              Reimport,
              (const std::filesystem::path& assetPath),
              (override));
  MOCK_METHOD(std::unique_ptr<IAssetReference>,
              Retrieve,
              (const UUID& uid),
              (override));
  MOCK_METHOD(std::unique_ptr<IAssetReference>,
              Retrieve,
              (const std::filesystem::path& path),
              (override));
  MOCK_METHOD(entt::registry&, GetRegistry, (), (override));
  MOCK_METHOD(void,
              Rename,
              (const std::filesystem::path& from,
               const std::filesystem::path& to),
              (override));
  MOCK_METHOD(void,
              RenameDirectory,
              (const std::filesystem::path& from,
               const std::filesystem::path& to),
              (override));
  MOCK_METHOD(std::filesystem::path, GetAssetDirectoryPath, (), (override));
};

class AssetReimporterTest : public ::testing::Test
{
protected:
  std::shared_ptr<MockLogger>        logger;
  std::shared_ptr<MockAssetDatabase> assetDatabase;
  std::shared_ptr<AssetReimporter>   assetReimporter;

  void
  SetUp() override
  {
    auto injector = boost::di::make_injector(
      boost::di::bind<IDwarfLogger>().to<MockLogger>(),
      boost::di::bind<IAssetDatabase>().to<MockAssetDatabase>());

    assetReimporter = injector.create<std::shared_ptr<AssetReimporter>>();
    logger = injector.create<std::shared_ptr<MockLogger>>();
    assetDatabase = injector.create<std::shared_ptr<MockAssetDatabase>>();
  }

  void
  TearDown() override
  {
    assetReimporter.reset();
    logger.reset();
    assetDatabase.reset();
  }
};

TEST_F(AssetReimporterTest, QueueReimport)
{
  std::filesystem::path assetPath = "test_asset_path";
  EXPECT_CALL(*logger,
              LogInfo(Log("Queued reimport for asset: test_asset_path",
                          "AssetReimporter")));

  assetReimporter->QueueReimport(assetPath);

  // Check if the asset path is in the reimport queue
  // This requires access to the private member mReimportQueue, which can be
  // done via a friend class or reflection in real scenarios.
}

TEST_F(AssetReimporterTest, ReimportQueuedAssets)
{
  std::filesystem::path assetPath1 = "test_asset_path1";
  std::filesystem::path assetPath2 = "test_asset_path2";

  EXPECT_CALL(*logger,
              LogInfo(Log("Queued reimport for asset: test_asset_path1",
                          "AssetReimporter")));
  EXPECT_CALL(*logger,
              LogInfo(Log("Queued reimport for asset: test_asset_path2",
                          "AssetReimporter")));
  EXPECT_CALL(
    *logger,
    LogInfo(Log("Reimporting asset: test_asset_path1", "AssetReimporter")));
  EXPECT_CALL(
    *logger,
    LogInfo(Log("Reimporting asset: test_asset_path2", "AssetReimporter")));
  EXPECT_CALL(*assetDatabase, Reimport(assetPath1));
  EXPECT_CALL(*assetDatabase, Reimport(assetPath2));

  assetReimporter->QueueReimport(assetPath1);
  assetReimporter->QueueReimport(assetPath2);
  assetReimporter->ReimportQueuedAssets();

  // Check if the reimport queue is empty
  // This requires access to the private member mReimportQueue, which can be
  // done via a friend class or reflection in real scenarios.
}