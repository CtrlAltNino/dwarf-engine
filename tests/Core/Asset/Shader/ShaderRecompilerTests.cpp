#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Core/Asset/Shader/ShaderRecompiler.h"
#include "Core/Rendering/Shader/IShader.h"

using namespace Dwarf;
using namespace testing;

// Mock class for IShader
class MockShader : public IShader
{
public:
  MOCK_METHOD(void, Compile, (), (override));
  MOCK_METHOD(bool, IsCompiled, (), (const, override));
  MOCK_METHOD(std::unique_ptr<IShaderParameterCollection>,
              CreateParameters,
              (),
              (override));
  MOCK_METHOD(nlohmann::json, Serialize, (), (const, override));
};

class ShaderRecompilerTest : public Test
{
protected:
  ShaderRecompiler* recompiler;

  void
  SetUp() override
  {
    recompiler = new ShaderRecompiler();
  }

  void
  TearDown() override
  {
    delete recompiler;
  }
};

TEST_F(ShaderRecompilerTest, MarkForRecompilation)
{
  auto mockShader = std::make_shared<MockShader>();

  // Call the method under test
  recompiler->MarkForRecompilation(mockShader);

  // Verify the shader is marked for recompilation
  // Since m_ShadersToRecompile is private, we can't directly access it.
  // We will verify it indirectly by calling Recompile and checking if Compile
  // is called.
  EXPECT_CALL(*mockShader, Compile()).Times(1);

  recompiler->Recompile();
}

TEST_F(ShaderRecompilerTest, Recompile)
{
  auto mockShader1 = std::make_shared<MockShader>();
  auto mockShader2 = std::make_shared<MockShader>();

  // Mark shaders for recompilation
  recompiler->MarkForRecompilation(mockShader1);
  recompiler->MarkForRecompilation(mockShader2);

  // Set expectations
  EXPECT_CALL(*mockShader1, Compile()).Times(1);
  EXPECT_CALL(*mockShader2, Compile()).Times(1);

  // Call the method under test
  recompiler->Recompile();

  // Verify that the shaders are recompiled and the list is cleared
  // Since m_ShadersToRecompile is private, we can't directly access it.
  // We will verify it indirectly by calling Recompile again and checking if
  // Compile is not called.
  recompiler->Recompile();
}