#include "MaterialFactory.h"
#include "Material.h"

namespace Dwarf
{
  MaterialFactory::MaterialFactory(
    std::shared_ptr<IDwarfLogger>   logger,
    std::shared_ptr<IShaderFactory> shaderFactory,
    std::shared_ptr<IShaderParameterCollectionFactory>
      shaderParameterCollectionFactory)
    : m_Logger(logger)
    , m_ShaderFactory(shaderFactory)
    , m_ShaderParameterCollectionFactory(shaderParameterCollectionFactory)
  {
    m_Logger->LogInfo(Log("MaterialFactory created", "MaterialFactory"));
  }

  MaterialFactory::~MaterialFactory()
  {
    m_Logger->LogInfo(Log("MaterialFactory destroyed", "MaterialFactory"));
  }

  std::unique_ptr<IMaterial>
  MaterialFactory::CreateDefaultMaterial() const
  {
    // TODO: Use default shader
    m_Logger->LogInfo(Log("Creating default material", "MaterialFactory"));
    return std::make_unique<Material>(
      m_ShaderFactory->CreateDefaultShader(),
      MaterialProperties(),
      m_ShaderParameterCollectionFactory->CreateShaderParameterCollection());
  }

  std::unique_ptr<IMaterial>
  MaterialFactory::CreateMaterial(std::unique_ptr<IShader> shader) const
  {
    m_Logger->LogInfo(Log("Creating material", "MaterialFactory"));
    return std::make_unique<Material>(
      std::move(shader),
      MaterialProperties(),
      m_ShaderParameterCollectionFactory->CreateShaderParameterCollection());
  }

  std::unique_ptr<IMaterial>
  MaterialFactory::FromSerialized(
    const nlohmann::json& serializedMaterial) const
  {
    m_Logger->LogInfo(
      Log("Creating material from serialized data", "MaterialFactory"));
    return std::make_unique<Material>(
      serializedMaterial.contains("Shader")
        ? m_ShaderFactory->CreateShader(serializedMaterial["Shader"])
        : m_ShaderFactory->CreateDefaultShader(),
      serializedMaterial["Properties"],
      m_ShaderParameterCollectionFactory->CreateShaderParameterCollection(
        serializedMaterial["ShaderParameters"]));
  }
}