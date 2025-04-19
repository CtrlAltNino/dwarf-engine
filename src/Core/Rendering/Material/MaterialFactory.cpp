#include "pch.hpp"

#include "Material.hpp"
#include "MaterialFactory.hpp"

namespace Dwarf
{
  MaterialFactory::MaterialFactory(
    std::shared_ptr<IDwarfLogger> logger,
    std::shared_ptr<IShaderParameterCollectionFactory>
                                     shaderParameterCollectionFactory,
    std::shared_ptr<IShaderRegistry> shaderRegistry,
    std::shared_ptr<IShaderSourceCollectionFactory>
      shaderSourceCollectionFactory,
    std::shared_ptr<IShaderAssetSourceContainerFactory>
      shaderAssetSourceContainerFactory)
    : mLogger(std::move(logger))
    , mShaderParameterCollectionFactory(
        std::move(shaderParameterCollectionFactory))
    , mShaderRegistry(std::move(shaderRegistry))
    , mShaderSourceCollectionFactory(std::move(shaderSourceCollectionFactory))
    , mShaderAssetSourceContainerFactory(
        std::move(shaderAssetSourceContainerFactory))
  {
    mLogger->LogDebug(Log("MaterialFactory created", "MaterialFactory"));
  }

  MaterialFactory::~MaterialFactory()
  {
    mLogger->LogDebug(Log("MaterialFactory destroyed", "MaterialFactory"));
  }

  auto
  MaterialFactory::CreateDefaultMaterial() const -> std::unique_ptr<IMaterial>
  {
    mLogger->LogDebug(Log("Creating default material", "MaterialFactory"));
    return std::make_unique<Material>(
      MaterialProperties(),
      mShaderParameterCollectionFactory->Create(),
      mShaderAssetSourceContainerFactory->Create(
        mShaderSourceCollectionFactory->CreateDefaultShaderSourceCollection()),
      mShaderRegistry);
  }

  auto
  MaterialFactory::CreateMaterial(
    std::unique_ptr<IShaderSourceCollection> shaderSources) const
    -> std::unique_ptr<IMaterial>
  {
    mLogger->LogDebug(Log("Creating default material", "MaterialFactory"));
    return std::make_unique<Material>(
      MaterialProperties(),
      mShaderParameterCollectionFactory->Create(),
      mShaderAssetSourceContainerFactory->Create(std::move(shaderSources)),
      mShaderRegistry);
  }

  auto
  MaterialFactory::FromSerialized(const nlohmann::json& serializedMaterial)
    const -> std::unique_ptr<IMaterial>
  {
    mLogger->LogDebug(
      Log("Creating material from serialized data", "MaterialFactory"));
    return std::make_unique<Material>(
      serializedMaterial["Properties"],
      mShaderParameterCollectionFactory->FromSerialized(
        serializedMaterial["ShaderParameters"]),
      mShaderAssetSourceContainerFactory->Create(
        serializedMaterial.contains("Shader")
          ? mShaderSourceCollectionFactory->CreateShaderSourceCollection(
              serializedMaterial["Shader"])
          : mShaderSourceCollectionFactory
              ->CreateDefaultShaderSourceCollection()),
      mShaderRegistry);
  }
}