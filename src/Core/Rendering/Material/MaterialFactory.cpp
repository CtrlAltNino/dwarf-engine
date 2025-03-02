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
    : m_Logger(logger)
    , m_ShaderParameterCollectionFactory(shaderParameterCollectionFactory)
    , m_ShaderRegistry(shaderRegistry)
    , m_ShaderSourceCollectionFactory(shaderSourceCollectionFactory)
  {
    m_Logger->LogDebug(Log("MaterialFactory created", "MaterialFactory"));
  }

  MaterialFactory::~MaterialFactory()
  {
    m_Logger->LogDebug(Log("MaterialFactory destroyed", "MaterialFactory"));
  }

  std::unique_ptr<IMaterial>
  MaterialFactory::CreateDefaultMaterial() const
  {
    // TODO: Use default shader
    m_Logger->LogDebug(Log("Creating default material", "MaterialFactory"));
    return std::make_unique<Material>(
      m_ShaderRegistry->GetOrCreate(
        m_ShaderSourceCollectionFactory->CreateDefaultShaderSourceCollection()),
      MaterialProperties(),
      m_ShaderParameterCollectionFactory->CreateShaderParameterCollection());
  }

  std::unique_ptr<IMaterial>
  MaterialFactory::CreateMaterial(std::shared_ptr<IShader> shader) const
  {
    m_Logger->LogDebug(Log("Creating material", "MaterialFactory"));
    return std::make_unique<Material>(
      shader,
      MaterialProperties(),
      m_ShaderParameterCollectionFactory->CreateShaderParameterCollection());
  }

  std::unique_ptr<IMaterial>
  MaterialFactory::FromSerialized(
    const nlohmann::json& serializedMaterial) const
  {
    m_Logger->LogDebug(
      Log("Creating material from serialized data", "MaterialFactory"));
    return std::make_unique<Material>(
      serializedMaterial.contains("Shader")
        ? m_ShaderRegistry->GetOrCreate(
            m_ShaderSourceCollectionFactory->CreateShaderSourceCollection(
              serializedMaterial["Shader"]))
        : m_ShaderRegistry->GetOrCreate(
            m_ShaderSourceCollectionFactory
              ->CreateDefaultShaderSourceCollection()),
      serializedMaterial["Properties"],
      m_ShaderParameterCollectionFactory->CreateShaderParameterCollection(
        serializedMaterial["ShaderParameters"]));
  }
}