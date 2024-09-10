#include "Core/Rendering/Material/IMaterial.h"
#include <Core/Rendering/Material/Material.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
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
              (std::string_view identifier, Dwarf::ParameterValue parameter),
              (override));
  MOCK_METHOD(std::optional<Dwarf::ParameterValue>,
              GetParameter,
              (std::string const& name),
              (const, override));
  MOCK_METHOD(const std::vector<std::string>,
              GetParameterIdentifiers,
              (),
              (const, override));
  MOCK_METHOD(void, RemoveParameter, (std::string const& name), (override));
  MOCK_METHOD(bool, HasParameter, (std::string const& name), (const, override));
  MOCK_METHOD(void, ClearParameters, (), (override));
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
};

TEST(MaterialTests, EmptyShaderInitialization)
{
  Dwarf::Material material(nullptr);
  ASSERT_EQ(material.GetShader(), nullptr);
}

TEST(MaterialTests, DefaultShaderInitialization)
{
  auto            shader = std::make_shared<MockIShader>();
  Dwarf::Material material(shader);
  ASSERT_EQ(material.GetShader(), shader);
}

TEST(MaterialTests, DefaultMaterialProperties)
{
  auto            shader = std::make_shared<MockIShader>();
  Dwarf::Material material(shader);
  auto            properties = material.GetMaterialProperties();
  ASSERT_FALSE(properties.IsTransparent);
  ASSERT_FALSE(properties.IsDoubleSided);
  ASSERT_FALSE(properties.IsUnlit);
  ASSERT_FALSE(properties.IsWireframe);
}

TEST(MaterialTests, CustomMaterialProperties)
{
  auto                      shader = std::make_shared<MockIShader>();
  Dwarf::MaterialProperties sourceProperties(false, true, true, false);
  Dwarf::Material           material(shader, sourceProperties);
  auto                      properties = material.GetMaterialProperties();
  ASSERT_FALSE(properties.IsTransparent);
  ASSERT_TRUE(properties.IsDoubleSided);
  ASSERT_TRUE(properties.IsUnlit);
  ASSERT_FALSE(properties.IsWireframe);
}

TEST(MaterialTests, MaterialSerialization)
{
  auto            shader = std::make_shared<MockIShader>();
  Dwarf::Material material(shader);
  auto            serialized = material.Serialize();
  ASSERT_FALSE(serialized.empty());
  ASSERT_TRUE(serialized.contains("shader"));
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
