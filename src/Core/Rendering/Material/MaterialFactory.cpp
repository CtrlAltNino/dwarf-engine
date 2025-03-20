#include "MaterialFactory.h"
#include "Material.h"
#include <memory>

namespace Dwarf
{
  MaterialFactory::MaterialFactory(
    std::shared_ptr<IDwarfLogger> logger,
    std::shared_ptr<IShaderParameterCollectionFactory>
                                     shaderParameterCollectionFactory,
    std::shared_ptr<IShaderRegistry> shaderRegistry,
    std::shared_ptr<IShaderSourceCollectionFactory>
      shaderSourceCollectionFactory)
    : mLogger(logger)
    , mShaderParameterCollectionFactory(shaderParameterCollectionFactory)
    , mShaderRegistry(shaderRegistry)
    , mShaderSourceCollectionFactory(shaderSourceCollectionFactory)
  {
    mLogger->LogDebug(Log("MaterialFactory created", "MaterialFactory"));
  }

  MaterialFactory::~MaterialFactory()
  {
    mLogger->LogDebug(Log("MaterialFactory destroyed", "MaterialFactory"));
  }

  std::unique_ptr<IMaterial>
  MaterialFactory::CreateDefaultMaterial() const
  {
    // TODO: Use default shader
    mLogger->LogDebug(Log("Creating default material", "MaterialFactory"));
    return std::make_unique<Material>(
      mShaderRegistry->GetOrCreate(
        mShaderSourceCollectionFactory->CreateDefaultShaderSourceCollection()),
      MaterialProperties(),
      mShaderParameterCollectionFactory->CreateShaderParameterCollection());
  }

  std::unique_ptr<IMaterial>
  MaterialFactory::CreateMaterial(std::shared_ptr<IShader> shader) const
  {
    mLogger->LogDebug(Log("Creating material", "MaterialFactory"));
    return std::make_unique<Material>(
      shader,
      MaterialProperties(),
      mShaderParameterCollectionFactory->CreateShaderParameterCollection());
  }

  std::unique_ptr<IMaterial>
  MaterialFactory::FromSerialized(
    const nlohmann::json& serializedMaterial) const
  {
    mLogger->LogDebug(
      Log("Creating material from serialized data", "MaterialFactory"));
    return std::make_unique<Material>(
      serializedMaterial.contains("Shader")
        ? mShaderRegistry->GetOrCreate(
            mShaderSourceCollectionFactory->CreateShaderSourceCollection(
              serializedMaterial["Shader"]))
        : mShaderRegistry->GetOrCreate(
            mShaderSourceCollectionFactory
              ->CreateDefaultShaderSourceCollection()),
      serializedMaterial["Properties"],
      mShaderParameterCollectionFactory->CreateShaderParameterCollection(
        serializedMaterial["ShaderParameters"]));
  }
}