#include "MaterialFactory.h"
#include "Material.h"
#include <memory>
#include <utility>

namespace Dwarf
{
  MaterialFactory::MaterialFactory(
    std::shared_ptr<IDwarfLogger> logger,
    std::shared_ptr<IShaderParameterCollectionFactory>
                                     shaderParameterCollectionFactory,
    std::shared_ptr<IShaderRegistry> shaderRegistry,
    std::shared_ptr<IShaderSourceCollectionFactory>
      shaderSourceCollectionFactory)
    : mLogger(std::move(logger))
    , mShaderParameterCollectionFactory(
        std::move(shaderParameterCollectionFactory))
    , mShaderRegistry(std::move(shaderRegistry))
    , mShaderSourceCollectionFactory(std::move(shaderSourceCollectionFactory))
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
    // TODO: Use default shader
    mLogger->LogDebug(Log("Creating default material", "MaterialFactory"));
    return std::make_unique<Material>(
      mShaderRegistry->GetOrCreate(
        mShaderSourceCollectionFactory->CreateDefaultShaderSourceCollection()),
      MaterialProperties(),
      mShaderParameterCollectionFactory->Create());
  }

  auto
  MaterialFactory::CreateMaterial(std::shared_ptr<IShader> shader) const
    -> std::unique_ptr<IMaterial>
  {
    mLogger->LogDebug(Log("Creating material", "MaterialFactory"));
    return std::make_unique<Material>(
      shader,
      MaterialProperties(),
      mShaderParameterCollectionFactory->Create());
  }

  auto
  MaterialFactory::FromSerialized(const nlohmann::json& serializedMaterial)
    const -> std::unique_ptr<IMaterial>
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
      mShaderParameterCollectionFactory->FromSerialized(
        serializedMaterial["ShaderParameters"]));
  }
}