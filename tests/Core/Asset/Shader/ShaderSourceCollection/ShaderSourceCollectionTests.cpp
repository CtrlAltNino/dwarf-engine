#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Core/Asset/Shader/ShaderSourceCollection/ShaderSourceCollection.h"
#include "Core/Asset/AssetReference/IAssetReference.h"

using namespace Dwarf;
using namespace testing;

// Mock class for IAssetReference
class MockAssetReference : public IAssetReference
{
public:
  MOCK_METHOD(entt::entity, GetHandle, (), (const, override));
  MOCK_METHOD(const UUID&, GetUID, (), (const, override));
  MOCK_METHOD(const std::filesystem::path&, GetPath, (), (const, override));
  MOCK_METHOD(IAssetComponent&, GetAsset, (), (override));
  MOCK_METHOD(ASSET_TYPE, GetType, (), (const, override));
  MOCK_METHOD(std::unique_ptr<IAssetReference>, Clone, (), (const, override));
};

class ShaderSourceCollectionTest : public Test
{
protected:
  std::vector<std::unique_ptr<IAssetReference>> shaderSources;

  void
  SetUp() override
  {
    // Create mock shader sources
    auto mockVertexShader = std::make_unique<MockAssetReference>();
    auto mockFragmentShader = std::make_unique<MockAssetReference>();

    shaderSources.push_back(std::move(mockVertexShader));
    shaderSources.push_back(std::move(mockFragmentShader));
  }
};

TEST_F(ShaderSourceCollectionTest, Constructor)
{
  // Create ShaderSourceCollection
  ShaderSourceCollection shaderSourceCollection(shaderSources);

  // Verify the shader sources
  auto& sources = shaderSourceCollection.GetShaderSources();
  ASSERT_EQ(sources.size(), 2);
}

TEST_F(ShaderSourceCollectionTest, GetShaderSources)
{
  // Create ShaderSourceCollection
  ShaderSourceCollection shaderSourceCollection(shaderSources);

  // Verify the shader sources
  auto& sources = shaderSourceCollection.GetShaderSources();
  ASSERT_EQ(sources.size(), 2);
}