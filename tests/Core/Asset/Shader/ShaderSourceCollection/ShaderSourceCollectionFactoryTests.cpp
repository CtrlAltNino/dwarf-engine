#include "Core/Asset/AssetReference/IAssetReference.hpp"
#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Asset/Shader/ShaderSourceCollection/ShaderSourceCollection.hpp"
#include "Core/Asset/Shader/ShaderSourceCollection/ShaderSourceCollectionFactory.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <nlohmann/json.hpp>

using namespace Dwarf;
using namespace testing;

// Mock class for IAssetDatabase
class MockAssetDatabase : public IAssetDatabase
{
public:
  MOCK_METHOD(UUID, Import, (const std::filesystem::path&), (override));
  MOCK_METHOD(void, ImportDialog, (), (override));
  MOCK_METHOD(bool, Exists, (const UUID&), (override));
  MOCK_METHOD(bool, Exists, (const std::filesystem::path&), (override));
  MOCK_METHOD(void, Clear, (), (override));
  MOCK_METHOD(void, Remove, (const UUID&), (override));
  MOCK_METHOD(void, Remove, (const std::filesystem::path&), (override));
  MOCK_METHOD(void, ReimportAll, (), (override));
  MOCK_METHOD(void, Reimport, (const std::filesystem::path&), (override));
  MOCK_METHOD(std::unique_ptr<IAssetReference>,
              Retrieve,
              (const UUID&),
              (override));
  MOCK_METHOD(std::unique_ptr<IAssetReference>,
              Retrieve,
              (const std::filesystem::path&),
              (override));
  MOCK_METHOD(entt::registry&, GetRegistry, (), (override));
  MOCK_METHOD(void,
              Rename,
              (const std::filesystem::path&, const std::filesystem::path&),
              (override));
  MOCK_METHOD(void,
              RenameDirectory,
              (const std::filesystem::path&, const std::filesystem::path&),
              (override));
  MOCK_METHOD(void,
              RegisterAssetDatabaseObserver,
              (IAssetDatabaseObserver * observer),
              (override));
  MOCK_METHOD(void,
              UnregisterAssetDatabaseObserver,
              (IAssetDatabaseObserver * observer),
              (override));
};

// Mock class for IAssetReference
class MockAssetReference : public IAssetReference
{
public:
  MOCK_METHOD(entt::entity, GetHandle, (), (const, override));
  MOCK_METHOD(const UUID&, GetUID, (), (const, override));
  MOCK_METHOD(const std::filesystem::path&, GetPath, (), (const, override));
  MOCK_METHOD(IAssetComponent&, GetAsset, (), (override));
  MOCK_METHOD(ASSET_TYPE, GetType, (), (const, override));
  MOCK_METHOD(bool, IsValid, (), (const, override));
};

class ShaderSourceCollectionFactoryTest : public Test
{
protected:
  std::shared_ptr<MockAssetDatabase>             mockAssetDatabase;
  std::shared_ptr<ShaderSourceCollectionFactory> factory;

  void
  SetUp() override
  {
    // Initialize the mock asset database

    // Create a boost di injector
    auto injector = boost::di::make_injector(
      boost::di::bind<IAssetDatabase>().to<MockAssetDatabase>(),
      boost::di::bind<GraphicsApi>().to(GraphicsApi::OpenGL));

    // Create the factory using the inj>ector
    factory = injector.create<std::shared_ptr<ShaderSourceCollectionFactory>>();
    mockAssetDatabase = injector.create<std::shared_ptr<MockAssetDatabase>>();
  }

  void
  TearDown() override
  {
    factory.reset();
  }
};

TEST_F(ShaderSourceCollectionFactoryTest,
       CreateDefaultShaderSourceCollection_OpenGL)
{
  // Setup mock expectations
  auto mockVertexShader = std::make_unique<MockAssetReference>();
  auto mockFragmentShader = std::make_unique<MockAssetReference>();

  ASSERT_NE(mockAssetDatabase, nullptr);
  ASSERT_NE(factory, nullptr);

  EXPECT_CALL(*mockAssetDatabase,
              Retrieve(std::filesystem::path(
                "data/engine/shaders/default/opengl/default.vert")))
    .WillOnce(Return(ByMove(std::move(mockVertexShader))));
  EXPECT_CALL(*mockAssetDatabase,
              Retrieve(std::filesystem::path(
                "data/engine/shaders/default/opengl/default.frag")))
    .WillOnce(Return(ByMove(std::move(mockFragmentShader))));

  // Call the method under test
  auto shaderCollection = factory->CreateDefaultShaderSourceCollection();

  // Verify the result
  ASSERT_NE(shaderCollection, nullptr);
}

TEST_F(ShaderSourceCollectionFactoryTest, CreateShaderSourceCollection_FromJson)
{
  // Setup mock expectations
  auto mockVertexShader = std::make_unique<MockAssetReference>();
  auto mockFragmentShader = std::make_unique<MockAssetReference>();

  UUID vertexShaderId("e1e18e37-e972-4483-b6ed-086aee3c8654");
  UUID fragmentShaderId("c96c6f37-8df5-46ef-8264-2fbd3e2d72ca");

  EXPECT_CALL(*mockAssetDatabase, Retrieve(vertexShaderId))
    .WillOnce(Return(ByMove(std::move(mockVertexShader))));
  EXPECT_CALL(*mockAssetDatabase, Retrieve(fragmentShaderId))
    .WillOnce(Return(ByMove(std::move(mockFragmentShader))));

  // Create a JSON object
  nlohmann::json json = {
    { "vertexShader", "e1e18e37-e972-4483-b6ed-086aee3c8654" },
    { "fragmentShader", "c96c6f37-8df5-46ef-8264-2fbd3e2d72ca" }
  };

  // Call the method under test
  auto shaderCollection = factory->CreateShaderSourceCollection(json);

  // Verify the result
  ASSERT_NE(shaderCollection, nullptr);
}