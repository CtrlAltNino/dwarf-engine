#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.hpp"
#include "Core/Rendering/Material/IMaterial.hpp"
#include "Core/Rendering/Material/Material.hpp"
#include "Core/Rendering/Material/ShaderAssetSourceContainer/IShaderAssetSourceContainer.hpp"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>


TEST(MaterialTests, Sample)
{
  const auto expected = 1;
  //   const auto actual = multiply(1, 1);
  ASSERT_EQ(expected, 1);
}

class MockIShaderParameterCollection : public Dwarf::IShaderParameterCollection
{
public:
  MOCK_METHOD(void,
              SetParameter,
              (std::string_view              identifier,
               Dwarf::MaterialParameterValue parameter),
              (override));
  MOCK_METHOD(Dwarf::MaterialParameterValue&,
              GetParameter,
              (const std::string& name),
              (override));
  MOCK_METHOD(const std::vector<std::string>,
              GetParameterIdentifiers,
              (),
              (const, override));
  MOCK_METHOD(void,
              PatchParameters,
              (const std::unique_ptr<IShaderParameterCollection>& parameters),
              (override));
  MOCK_METHOD(void, RemoveParameter, (std::string const& name), (override));
  MOCK_METHOD(bool, HasParameter, (std::string const& name), (const, override));
  MOCK_METHOD(void, ClearParameters, (), (override));
  MOCK_METHOD(nlohmann::json, Serialize, (), (override));
};

class MockIShader : public Dwarf::IShader
{
public:
  MOCK_METHOD(void, Compile, (), (override));
  MOCK_METHOD(bool, IsCompiled, (), (const, override));
  MOCK_METHOD(std::unique_ptr<Dwarf::IShaderParameterCollection>,
              CreateParameters,
              (),
              (override));
  MOCK_METHOD(bool, CompareTo, (const IShader& other), (const));
  bool
  operator<(const IShader& other) const override
  {
    return CompareTo(other);
  }
};

class MockIShaderRegistry : public Dwarf::IShaderRegistry
{
public:
  MOCK_METHOD(std::shared_ptr<Dwarf::IShader>,
              GetOrCreate,
              (std::unique_ptr<Dwarf::IShaderSourceCollection> shaderSources),
              (override));
};

class MockIShaderAssetSourceContainer
  : public Dwarf::IShaderAssetSourceContainer
{
public:
  MOCK_METHOD(std::unique_ptr<Dwarf::IShaderSourceCollection>,
              GetShaderSources,
              (),
              (override));
  MOCK_METHOD(nlohmann::json, Serialize, (), (override));
};

// Testing if the default shader is initialized correctly
TEST(MaterialTests, DefaultShaderInitialization)
{
  std::shared_ptr<MockIShaderRegistry> shaderRegistry =
    std::make_shared<MockIShaderRegistry>();

  std::unique_ptr<MockIShaderAssetSourceContainer> shaderSourceCollection =
    std::make_unique<MockIShaderAssetSourceContainer>();

  // Expect a Compile call on the shader

  Dwarf::Material material(Dwarf::MaterialProperties(),
                           nullptr,
                           std::move(shaderSourceCollection),
                           shaderRegistry);
}

// Testing if the default material properties are initialized correctly
TEST(MaterialTests, DefaultMaterialProperties)
{
  std::shared_ptr<MockIShaderRegistry> shaderRegistry =
    std::make_shared<MockIShaderRegistry>();

  std::unique_ptr<MockIShaderAssetSourceContainer> shaderSourceCollection =
    std::make_unique<MockIShaderAssetSourceContainer>();
  Dwarf::Material material(Dwarf::MaterialProperties(),
                           nullptr,
                           std::move(shaderSourceCollection),
                           shaderRegistry);
  auto            properties = material.GetMaterialProperties();
  ASSERT_FALSE(properties.IsTransparent);
  ASSERT_FALSE(properties.IsDoubleSided);
  ASSERT_FALSE(properties.IsUnlit);
  ASSERT_FALSE(properties.IsWireframe);
}

// Testing if the custom material properties are initialized correctly
TEST(MaterialTests, CustomMaterialProperties)
{
  std::shared_ptr<MockIShaderRegistry> shaderRegistry =
    std::make_shared<MockIShaderRegistry>();

  std::unique_ptr<MockIShaderAssetSourceContainer> shaderSourceCollection =
    std::make_unique<MockIShaderAssetSourceContainer>();
  Dwarf::MaterialProperties sourceProperties(false, true, true, false);
  Dwarf::Material           material(sourceProperties,
                           nullptr,
                           std::move(shaderSourceCollection),
                           shaderRegistry);
  auto                      properties = material.GetMaterialProperties();
  ASSERT_FALSE(properties.IsTransparent);
  ASSERT_TRUE(properties.IsDoubleSided);
  ASSERT_TRUE(properties.IsUnlit);
  ASSERT_FALSE(properties.IsWireframe);
}

// Testing serialization of the material
TEST(MaterialTests, MaterialSerialization)
{
  std::shared_ptr<MockIShaderRegistry> shaderRegistry =
    std::make_shared<MockIShaderRegistry>();

  std::unique_ptr<MockIShaderAssetSourceContainer> shaderSourceCollection =
    std::make_unique<MockIShaderAssetSourceContainer>();
  auto shaderParameters = std::make_unique<MockIShaderParameterCollection>();
  Dwarf::Material material(Dwarf::MaterialProperties(),
                           std::move(shaderParameters),
                           std::move(shaderSourceCollection),
                           shaderRegistry);
  auto            serialized = material.Serialize();
  ASSERT_FALSE(serialized.empty());
  ASSERT_TRUE(serialized.contains("Shader"));
  ASSERT_TRUE(serialized.contains("Properties"));
  ASSERT_TRUE(serialized["Properties"]["IsTransparent"].is_boolean());
  ASSERT_TRUE(serialized["Properties"]["IsTransparent"].get<bool>() == false);

  ASSERT_TRUE(serialized["Properties"]["IsDoubleSided"].is_boolean());
  ASSERT_TRUE(serialized["Properties"]["IsDoubleSided"].get<bool>() == false);

  ASSERT_TRUE(serialized["Properties"]["IsUnlit"].is_boolean());
  ASSERT_TRUE(serialized["Properties"]["IsUnlit"].get<bool>() == false);

  ASSERT_TRUE(serialized["Properties"]["IsWireframe"].is_boolean());
  ASSERT_TRUE(serialized["Properties"]["IsWireframe"].get<bool>() == false);

  // ASSERT_TRUE(serialized.contains("ShaderParameters"));
}

// TEST(MaterialTests, ShaderInitializationWithProperties)
// {
//   auto            shader = std::make_shared<MockIShader>();
//   nlohmann::json  properties = { { "property1", "value1" },
//                                  { "property2", "value2" } };
//   Dwarf::Material material(shader, properties);
//   ASSERT_EQ(material.GetShader(), shader);
//   ASSERT_EQ(material.GetMaterialProperties(), properties);
// }

// TEST(MaterialTests, SetShader)
// {
//   auto            shader1 = std::make_shared<Dwarf::IShader>();
//   auto            shader2 = std::make_shared<Dwarf::IShader>();
//   Dwarf::Material material(shader1);
//   material.SetShader(shader2);
//   ASSERT_EQ(material.GetShader(), shader2);
// }

// TEST(MaterialTests, GenerateShaderParameters)
// {
//   auto            shader = std::make_shared<Dwarf::IShader>();
//   Dwarf::Material material(shader);
//   material.GenerateShaderParameters();
//   ASSERT_NE(material.GetShaderParameters(), nullptr);
// }

// TEST(MaterialTests, SerializeMaterial)
// {
//   auto            shader = std::make_shared<Dwarf::IShader>();
//   Dwarf::Material material(shader);
//   auto            serialized = material.Serialize();
//   ASSERT_TRUE(serialized.is_object());
// }
