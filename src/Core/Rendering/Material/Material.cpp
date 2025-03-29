#include "pch.h"

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.h"
#include "Material.h"

namespace Dwarf
{
  Material::Material(
    MaterialProperties                           materialProperties,
    std::unique_ptr<IShaderParameterCollection>  shaderParameters,
    std::unique_ptr<IShaderAssetSourceContainer> shaderAssetSourceContainer,
    const std::shared_ptr<IShaderRegistry>&      shaderRegistry)
    : mMaterialProperties(std::move(materialProperties))
    , mShaderParameters(std::move(shaderParameters))
    , mShaderAssetSourceContainer(std::move(shaderAssetSourceContainer))
    , mShaderRegistry(shaderRegistry)
  {
    UpdateShader();
  }

  auto
  Material::GetShader() -> std::shared_ptr<IShader>
  {
    return mShader;
  }

  auto
  Material::GetShaderParameters() const
    -> const std::unique_ptr<IShaderParameterCollection>&
  {
    return mShaderParameters;
  }

  void
  Material::UpdateShader()
  {
    mShader = mShaderRegistry.lock()->GetOrCreate(
      mShaderAssetSourceContainer->GetShaderSources());
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
  Material::GetShaderAssetSources()
    -> std::unique_ptr<IShaderAssetSourceContainer>&
  {
    return mShaderAssetSourceContainer;
  }

  auto
  Material::Serialize() -> nlohmann::json
  {
    nlohmann::json serializedMaterial;
    serializedMaterial["Shader"] = mShaderAssetSourceContainer->Serialize();
    serializedMaterial["Properties"] = mMaterialProperties.Serialize();
    serializedMaterial["ShaderParameters"] = mShaderParameters->Serialize();

    return serializedMaterial;
  }
}