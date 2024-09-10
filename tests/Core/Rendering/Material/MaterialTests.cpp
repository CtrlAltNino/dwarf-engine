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
  MOCK_METHOD(std::shared_ptr<Dwarf::IShaderParameterCollection>,
              GetParameters,
              (),
              (override));
};

TEST(MaterialTests, DefaultShaderInitialization)
{
  auto            shader = std::make_shared<MockIShader>();
  Dwarf::Material material(shader);
  ASSERT_EQ(material.GetShader(), shader);
}

// TEST(MaterialTests, ShaderInitializationWithProperties)
// {
//   auto            shader = std::make_shared<Dwarf::IShader>();
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
