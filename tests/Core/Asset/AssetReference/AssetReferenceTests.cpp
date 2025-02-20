#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/AssetReference/AssetReference.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/UUID.h"
#include <entt/entt.hpp>
#include <memory>
#include <filesystem>

using namespace Dwarf;

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
              (override));
  MOCK_METHOD(std::unique_ptr<ITexture>,
              FromData,
              (const std::shared_ptr<TextureContainer>& textureData),
              (override));
  MOCK_METHOD(std::unique_ptr<ITexture>,
              Empty,
              (const TextureType&       type,
               const TextureFormat&     format,
               const TextureDataType&   dataType,
               const TextureResolution& size,
               const TextureParameters& parameters,
               int                      samples),
              (override));
  MOCK_METHOD(std::unique_ptr<ITexture>,
              Empty,
              (const TextureType&       type,
               const TextureFormat&     format,
               const TextureDataType&   dataType,
               const TextureResolution& size,
               int                      samples),
              (override));
  MOCK_METHOD(std::shared_ptr<ITexture>, GetPlaceholderTexture, (), (override));
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

class AssetReferenceTest : public ::testing::Test
{
protected:
  entt::registry                      registry;
  entt::entity                        assetHandle;
  std::shared_ptr<MockModelImporter>  modelImporter;
  std::shared_ptr<MockTextureFactory> textureFactory;
  std::shared_ptr<MockMaterialIO>     materialIO;
  std::shared_ptr<MockFileHandler>    fileHandler;

  void
  SetUp() override
  {
    assetHandle = registry.create();
    modelImporter = std::make_shared<MockModelImporter>();
    textureFactory = std::make_shared<MockTextureFactory>();
    materialIO = std::make_shared<MockMaterialIO>();
    fileHandler = std::make_shared<MockFileHandler>();
  }
};

/*TEST_F(AssetReferenceTest, ConstructorForExistingAssets)
{
  ASSET_TYPE     type = ASSET_TYPE::MODEL;
  AssetReference assetRef(assetHandle,
                          registry,
                          type,
                          modelImporter,
                          textureFactory,
                          materialIO,
                          fileHandler);

  EXPECT_EQ(assetRef.GetHandle(), assetHandle);
  EXPECT_EQ(assetRef.GetType(), type);
}

TEST_F(AssetReferenceTest, ConstructorForNewAssets)
{
  UUID                  uid;
  std::filesystem::path path = "test_path";
  std::string           name = "test_name";
  AssetReference        assetRef(assetHandle,
                          registry,
                          uid,
                          path,
                          name,
                          modelImporter,
                          textureFactory,
                          materialIO,
                          fileHandler);

  EXPECT_EQ(assetRef.GetHandle(), assetHandle);
  EXPECT_EQ(assetRef.GetType(),
            IAssetDatabase::GetAssetType(path.extension().string()));
  EXPECT_EQ(assetRef.GetUID(), uid);
  EXPECT_EQ(assetRef.GetPath(), path);
}

TEST_F(AssetReferenceTest, GetHandle)
{
  ASSET_TYPE     type = ASSET_TYPE::MODEL;
  AssetReference assetRef(assetHandle,
                          registry,
                          type,
                          modelImporter,
                          textureFactory,
                          materialIO,
                          fileHandler);

  EXPECT_EQ(assetRef.GetHandle(), assetHandle);
}

TEST_F(AssetReferenceTest, GetUID)
{
  UUID                  uid;
  std::filesystem::path path = "test_path";
  std::string           name = "test_name";
  AssetReference        assetRef(assetHandle,
                          registry,
                          uid,
                          path,
                          name,
                          modelImporter,
                          textureFactory,
                          materialIO,
                          fileHandler);

  EXPECT_EQ(assetRef.GetUID(), uid);
}

TEST_F(AssetReferenceTest, GetPath)
{
  UUID                  uid;
  std::filesystem::path path = "test_path";
  std::string           name = "test_name";
  AssetReference        assetRef(assetHandle,
                          registry,
                          uid,
                          path,
                          name,
                          modelImporter,
                          textureFactory,
                          materialIO,
                          fileHandler);

  EXPECT_EQ(assetRef.GetPath(), path);
}

TEST_F(AssetReferenceTest, GetType)
{
  ASSET_TYPE     type = ASSET_TYPE::MODEL;
  AssetReference assetRef(assetHandle,
                          registry,
                          type,
                          modelImporter,
                          textureFactory,
                          materialIO,
                          fileHandler);

  EXPECT_EQ(assetRef.GetType(), type);
}

TEST_F(AssetReferenceTest, GetAsset)
{
  UUID                  uid;
  std::filesystem::path path = "test_path";
  std::string           name = "test_name";
  registry.emplace<ModelAsset>(assetHandle, modelImporter->Import(path));
  AssetReference assetRef(assetHandle,
                          registry,
                          uid,
                          path,
                          name,
                          modelImporter,
                          textureFactory,
                          materialIO,
                          fileHandler);

  EXPECT_NO_THROW(assetRef.GetAsset());
}*/