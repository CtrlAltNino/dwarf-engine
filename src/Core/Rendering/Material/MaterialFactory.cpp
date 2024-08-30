#include "MaterialFactory.h"
#include "Material.h"

namespace Dwarf
{
  MaterialFactory::MaterialFactory(
    std::shared_ptr<IShaderFactory> shaderFactory,
    std::shared_ptr<IShaderParameterCollectionFactory>
      shaderParameterCollectionFactory)
    : m_ShaderFactory(shaderFactory)
    , m_ShaderParameterCollectionFactory(shaderParameterCollectionFactory)
  {
  }

  MaterialFactory::~MaterialFactory() {}

  std::shared_ptr<IMaterial>
  MaterialFactory::CreateDefaultMaterial() const
  {
    // TODO: Use default shader
    return std::make_shared<Material>(m_ShaderFactory->CreateShader());
  }

  std::shared_ptr<IMaterial>
  MaterialFactory::CreateMaterial(std::shared_ptr<IShader> shader) const
  {
    return std::make_shared<Material>(shader);
  }

  std::shared_ptr<IMaterial>
  MaterialFactory::FromSerialized(
    const nlohmann::json& serializedMaterial) const
  {
    return std::make_shared<Material>(
      m_ShaderFactory->CreateShader(serializedMaterial["shader"]),
      serializedMaterial["materialProperties"]);
  }
}