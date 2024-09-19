#include "MaterialFactory.h"
#include "Material.h"

namespace Dwarf
{
  MaterialFactory::MaterialFactory(
    std::shared_ptr<IDwarfLogger>   logger,
    std::shared_ptr<IShaderFactory> shaderFactory)
    : m_Logger(logger)
    , m_ShaderFactory(shaderFactory)
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
    return std::make_unique<Material>(m_ShaderFactory->CreateDefaultShader());
  }

  std::unique_ptr<IMaterial>
  MaterialFactory::CreateMaterial(std::shared_ptr<IShader> shader) const
  {
    m_Logger->LogInfo(Log("Creating material", "MaterialFactory"));
    return std::make_unique<Material>(shader);
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
      serializedMaterial["Properties"]);
  }
}