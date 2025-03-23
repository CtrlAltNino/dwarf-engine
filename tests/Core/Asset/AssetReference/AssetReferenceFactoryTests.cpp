#include "Core/Asset/AssetReference/AssetReference.h"
#include "Core/Asset/AssetReference/AssetReferenceFactory.h"
#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/UUID.h"
#include "Logging/IDwarfLogger.h"
#include <entt/entt.hpp>
#include <filesystem>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace Dwarf;

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

class MockLogger : public IDwarfLogger
{
public:
  MOCK_METHOD(void, LogDebug, (const Log logMessage), (const, override));
  MOCK_METHOD(void, LogInfo, (const Log logMessage), (const, override));
  MOCK_METHOD(void, LogWarn, (const Log logMessage), (const, override));
  MOCK_METHOD(void, LogError, (const Log logMessage), (const, override));
};

class MockModelImporter : public IModelImporter
{
public:
  MOCK_METHOD(std::vector<std::unique_ptr<IMesh>>,
              Import,
              (const std::filesystem::path& path),
              (override));
};

class MockTextureFactory : public ITextureFactory
{
public:
  MOCK_METHOD(std::unique_ptr<ITexture>,
              FromPath,
              (std::filesystem::path texturePath),
              (const, override));
  MOCK_METHOD(std::unique_ptr<ITexture>,
              FromData,
              (const std::shared_ptr<TextureContainer>& textureData),
              (const, override));
  MOCK_METHOD(std::unique_ptr<ITexture>,
              Empty,
              (const TextureType&       type,
               const TextureFormat&     format,
               const TextureDataType&   dataType,
               const TextureResolution& size,
               const TextureParameters& parameters,
               int                      samples),
              (const, override));
  MOCK_METHOD(std::unique_ptr<ITexture>,
              Empty,
              (const TextureType&       type,
               const TextureFormat&     format,
               const TextureDataType&   dataType,
               const TextureResolution& size,
               uint32_t                 samples),
              (const, override));
  MOCK_METHOD(ITexture&, GetPlaceholderTexture, (), (override));
};

class MockMaterialIO : public IMaterialIO
{
public:
  MOCK_METHOD(void,
              SaveMaterial,
              (IMaterial & material, const std::filesystem::path& path),
              (const, override));
  MOCK_METHOD(std::unique_ptr<IMaterial>,
              LoadMaterial,
              (const std::filesystem::path& path),
              (const, override));
};

class MockTextureLoadingWorker : public ITextureLoadingWorker
{
public:
  MOCK_METHOD(void,
              RequestTextureLoad,
              (TextureLoadRequest request),
              (override));
  MOCK_METHOD(void,
              RequestTextureUpload,
              (TextureUploadRequest request),
              (override));
  MOCK_METHOD(void, ProcessTextureLoadRequests, (), (override));
  MOCK_METHOD(void, ProcessTextureJobs, (), (override));
  MOCK_METHOD(bool, IsRequested, (std::filesystem::path), (override));
};

class AssetReferenceFactoryTest : public ::testing::Test
{
protected:
  std::shared_ptr<MockLogger>               logger;
  std::shared_ptr<MockModelImporter>        modelImporter;
  std::shared_ptr<MockTextureFactory>       textureFactory;
  std::shared_ptr<MockMaterialIO>           materialIO;
  std::shared_ptr<MockFileHandler>          fileHandler;
  std::shared_ptr<MockTextureLoadingWorker> textureLoadingWorker;
  entt::registry                            registry;
  entt::entity                              assetHandle;

  void
  SetUp() override
  {
    logger = std::make_shared<MockLogger>();
    modelImporter = std::make_shared<MockModelImporter>();
    textureFactory = std::make_shared<MockTextureFactory>();
    materialIO = std::make_shared<MockMaterialIO>();
    fileHandler = std::make_shared<MockFileHandler>();
    textureLoadingWorker = std::make_shared<MockTextureLoadingWorker>();
    assetHandle = registry.create();
  }
};

TEST_F(AssetReferenceFactoryTest, Constructor)
{
  AssetReferenceFactory factory(logger,
                                modelImporter,
                                textureFactory,
                                materialIO,
                                fileHandler,
                                textureLoadingWorker);
  EXPECT_NE(&factory, nullptr);
}

TEST_F(AssetReferenceFactoryTest, Create)
{
  AssetReferenceFactory factory(logger,
                                modelImporter,
                                textureFactory,
                                materialIO,
                                fileHandler,
                                textureLoadingWorker);
  ASSET_TYPE            type = ASSET_TYPE::MODEL;
  auto                  assetRef = factory.Create(assetHandle, registry, type);

  EXPECT_NE(assetRef, nullptr);
  EXPECT_EQ(assetRef->GetHandle(), assetHandle);
  EXPECT_EQ(assetRef->GetType(), type);
}

TEST_F(AssetReferenceFactoryTest, CreateNew)
{
  AssetReferenceFactory factory(logger,
                                modelImporter,
                                textureFactory,
                                materialIO,
                                fileHandler,
                                textureLoadingWorker);
  UUID                  uid;
  std::filesystem::path path = "test_path";
  std::string           name = "test_name";
  auto assetRef = factory.CreateNew(assetHandle, registry, uid, path, name);

  EXPECT_NE(assetRef, nullptr);
  EXPECT_EQ(assetRef->GetHandle(), assetHandle);
  EXPECT_EQ(assetRef->GetUID(), uid);
  EXPECT_EQ(assetRef->GetPath(), path);
  EXPECT_EQ(assetRef->GetType(),
            IAssetDatabase::GetAssetType(path.extension().string()));
}