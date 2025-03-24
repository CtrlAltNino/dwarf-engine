#include "pch.h"

#include "Material.h"

namespace Dwarf
{
  Material::Material(
    std::shared_ptr<IShader>                    shader,
    MaterialProperties                          materialProperties,
    std::unique_ptr<IShaderParameterCollection> shaderParameters)
    : mShader(std::move(shader))
    , mMaterialProperties(std::move(materialProperties))
    , mShaderParameters(std::move(shaderParameters))
  {
    mShader->Compile();
  }

  auto
  Material::GetShader() -> IShader&
  {
    return *mShader;
  }

  void
  Material::SetShader(std::shared_ptr<IShader> shader)
  {
    mShader = shader;
  }

  auto
  Material::GetShaderParameters() const
    -> const std::unique_ptr<IShaderParameterCollection>&
  {
    return mShaderParameters;
  }

  auto
  Material::GetMaterialProperties() -> MaterialProperties&
  {
    return mMaterialProperties;
  }

  void
  Material::GenerateShaderParameters()
  {
    mShaderParameters = std::move(mShader->CreateParameters());
  }

  auto
  Material::Serialize() -> nlohmann::json
  {
    nlohmann::json serializedMaterial;
    serializedMaterial["Shader"] = mShader->Serialize();
    serializedMaterial["Properties"] = mMaterialProperties.Serialize();
    serializedMaterial["ShaderParameters"] = mShaderParameters->Serialize();

    return serializedMaterial;
  }
}