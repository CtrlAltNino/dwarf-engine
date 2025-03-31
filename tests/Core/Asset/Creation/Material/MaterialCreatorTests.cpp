#include "Core/Asset/Creation/Material/MaterialCreator.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include <filesystem>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace Dwarf;
using namespace testing;

class MockMaterial : public Dwarf::IMaterial
{
public:
  MOCK_METHOD(std::shared_ptr<Dwarf::IShader>, GetShader, (), (override));
  MOCK_METHOD(const std::unique_ptr<Dwarf::IShaderParameterCollection>&,
              GetShaderParameters,
              (),
              (const, override));
  MOCK_METHOD(Dwarf::MaterialProperties&,
              GetMaterialProperties,
              (),
              (override));
  MOCK_METHOD(void, GenerateShaderParameters, (), (override));
  MOCK_METHOD(void, UpdateShader, (), (override));
  MOCK_METHOD(std::unique_ptr<IShaderAssetSourceContainer>&,
              GetShaderAssetSources,
              (),
              (override));
  MOCK_METHOD(nlohmann::json, Serialize, (), (override));
};

class MockMaterialFactory : public IMaterialFactory
{
public:
  MOCK_METHOD(std::unique_ptr<IMaterial>,
              CreateDefaultMaterial,
              (),
              (const, override));
  MOCK_METHOD(std::unique_ptr<IMaterial>,
              FromSerialized,
              (const nlohmann::json& serializedMaterial),
              (const, override));
  MOCK_METHOD(std::unique_ptr<IMaterial>,
              CreateMaterial,
              (std::unique_ptr<IShaderSourceCollection> shaderSources),
              (const, override));
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

class MockShaderSourceCollectionFactory
  : public Dwarf::IShaderSourceCollectionFactory
{
public:
  MOCK_METHOD(std::unique_ptr<IShaderSourceCollection>,
              CreateDefaultShaderSourceCollection,
              (),
              (override));
  MOCK_METHOD(std::unique_ptr<IShaderSourceCollection>,
              CreateErrorShaderSourceCollection,
              (),
              (override));
  MOCK_METHOD(std::unique_ptr<IShaderSourceCollection>,
              CreateIdShaderSourceCollection,
              (),
              (override));
  MOCK_METHOD(std::unique_ptr<IShaderSourceCollection>,
              CreateGridShaderSourceCollection,
              (),
              (override));
  MOCK_METHOD(std::unique_ptr<IShaderSourceCollection>,
              CreateAgxTonemapShaderSourceCollection,
              (),
              (override));
  MOCK_METHOD(std::unique_ptr<IShaderSourceCollection>,
              CreatePreviewShaderSourceCollection,
              (),
              (override));
  MOCK_METHOD(std::unique_ptr<IShaderSourceCollection>,
              CreatePbrShaderSourceCollection,
              (),
              (override));
  MOCK_METHOD(std::unique_ptr<IShaderSourceCollection>,
              CreateUnlitShaderSourceCollection,
              (),
              (override));
  MOCK_METHOD(std::unique_ptr<IShaderSourceCollection>,
              CreateShaderSourceCollection,
              (const nlohmann::json& serializedShaderSourceCollection),
              (override));
  MOCK_METHOD(std::unique_ptr<IShaderSourceCollection>,
              CreateShaderSourceCollection,
              (std::vector<std::unique_ptr<IAssetReference>> & shaderSources),
              (override));
};

class MaterialCreatorTest : public ::testing::Test
{
protected:
  std::shared_ptr<MockMaterialFactory> materialFactory;
  std::shared_ptr<MockMaterialIO>      materialIO;
  std::shared_ptr<MockFileHandler>     fileHandler;
  std::shared_ptr<MockShaderSourceCollectionFactory>
                                   shaderSourceCollectionFactory;
  std::unique_ptr<MaterialCreator> materialCreator;
  AssetDirectoryPath               assetDirectoryPath;

  void
  SetUp() override
  {
    namespace di = boost::di;

    auto injector = di::make_injector(
      di::bind<IMaterialFactory>().to<MockMaterialFactory>().in(di::singleton),
      di::bind<IMaterialIO>().to<MockMaterialIO>().in(di::singleton),
      di::bind<IFileHandler>().to<MockFileHandler>().in(di::singleton),
      di::bind<IShaderSourceCollectionFactory>()
        .to<MockShaderSourceCollectionFactory>()
        .in(di::singleton),
      di::bind<AssetDirectoryPath>().to(
        AssetDirectoryPath{ "default/asset/directory" }));

    materialFactory = injector.create<std::shared_ptr<MockMaterialFactory>>();
    materialIO = injector.create<std::shared_ptr<MockMaterialIO>>();
    fileHandler = injector.create<std::shared_ptr<MockFileHandler>>();
    shaderSourceCollectionFactory =
      injector.create<std::shared_ptr<MockShaderSourceCollectionFactory>>();
    assetDirectoryPath = injector.create<AssetDirectoryPath>();
    materialCreator =
      std::make_unique<MaterialCreator>(assetDirectoryPath,
                                        materialFactory,
                                        materialIO,
                                        fileHandler,
                                        shaderSourceCollectionFactory);
  }
};

TEST_F(MaterialCreatorTest, CreateMaterialAsset_DefaultPath)
{
  auto mockMaterial = std::make_unique<MockMaterial>();
  EXPECT_CALL(*materialFactory, CreateDefaultMaterial())
    .WillOnce(Return(ByMove(std::move(mockMaterial))));
  EXPECT_CALL(*fileHandler, FileExists(_)).WillOnce(Return(false));
  EXPECT_CALL(
    *materialIO,
    SaveMaterial(
      _, std::filesystem::path("default/asset/directory/New Material.dmat")));

  materialCreator->CreateMaterialAsset(Dwarf::MaterialType::PbrMaterial,
                                       std::nullopt);
}

TEST_F(MaterialCreatorTest, CreateMaterialAsset_SpecifiedPath)
{
  auto mockMaterial = std::make_unique<MockMaterial>();
  EXPECT_CALL(*materialFactory, CreateDefaultMaterial())
    .WillOnce(Return(ByMove(std::move(mockMaterial))));
  EXPECT_CALL(*fileHandler, FileExists(_)).WillOnce(Return(false));
  EXPECT_CALL(
    *materialIO,
    SaveMaterial(_, std::filesystem::path("specified/path/New Material.dmat")));

  materialCreator->CreateMaterialAsset(Dwarf::MaterialType::PbrMaterial,
                                       "specified/path");
}

TEST_F(MaterialCreatorTest, CreateMaterialAsset_FileExists)
{
  auto mockMaterial = std::make_unique<MockMaterial>();
  EXPECT_CALL(*materialFactory, CreateDefaultMaterial())
    .WillOnce(Return(ByMove(std::move(mockMaterial))));
  EXPECT_CALL(*fileHandler,
              FileExists(std::filesystem::path(
                "default/asset/directory/New Material.dmat")))
    .WillOnce(Return(true));
  EXPECT_CALL(*fileHandler,
              FileExists(std::filesystem::path(
                "default/asset/directory/New Material (0).dmat")))
    .WillOnce(Return(false));
  EXPECT_CALL(*materialIO,
              SaveMaterial(_,
                           std::filesystem::path(
                             "default/asset/directory/New Material (0).dmat")));

  materialCreator->CreateMaterialAsset(Dwarf::MaterialType::PbrMaterial,
                                       std::nullopt);
}