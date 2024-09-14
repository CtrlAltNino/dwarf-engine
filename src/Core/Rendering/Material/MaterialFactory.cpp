#include "MaterialFactory.h"
#include "Material.h"

namespace Dwarf
{
  MaterialFactory::MaterialFactory(
    std::shared_ptr<IShaderFactory> shaderFactory)
    : m_ShaderFactory(shaderFactory)
  {
  }

  MaterialFactory::~MaterialFactory() {}

  std::unique_ptr<IMaterial>
  MaterialFactory::CreateDefaultMaterial() const
  {
    // TODO: Use default shader
    return std::make_unique<Material>(m_ShaderFactory->CreateDefaultShader());
  }

  std::unique_ptr<IMaterial>
  MaterialFactory::CreateMaterial(std::shared_ptr<IShader> shader) const
  {
    return std::make_unique<Material>(shader);
  }

  std::unique_ptr<IMaterial>
  MaterialFactory::FromSerialized(
    const nlohmann::json& serializedMaterial) const
  {
    return std::make_unique<Material>(
      m_ShaderFactory->CreateShader(serializedMaterial["shader"]),
      serializedMaterial["materialProperties"]);
  }
}