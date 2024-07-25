#include "MaterialFactory.h"
#include "Material.h"

namespace Dwarf
{
  MaterialFactory::MaterialFactory(
    std::shared_ptr<IShaderFactory>             shaderFactory,
    std::shared_ptr<IMaterialPropertiesFactory> materialPropertiesFactory,
    std::shared_ptr<IShaderParameterCollectionFactory>
      shaderParameterCollectionFactory)
    : m_ShaderFactory(shaderFactory)
    , m_MaterialPropertiesFactory(materialPropertiesFactory)
    , m_ShaderParameterCollectionFactory(shaderParameterCollectionFactory)
  {
  }

  MaterialFactory::~MaterialFactory() {}

  std::shared_ptr<IMaterial>
  MaterialFactory::CreateMaterial()
  {
    return std::make_shared<Material>(
      m_ShaderFactory->CreateShader(),
      m_MaterialPropertiesFactory->CreateMaterialProperties(),
      m_ShaderParameterCollectionFactory->CreateShaderParameterCollection());
    ;
  }

  std::shared_ptr<IMaterial>
  MaterialFactory::CreateMaterial(std::shared_ptr<IShader> shader)
  {
    return std::make_shared<Material>(
      shader,
      m_MaterialPropertiesFactory->CreateMaterialProperties(),
      m_ShaderParameterCollectionFactory->CreateShaderParameterCollection());
  }

  std::shared_ptr<IMaterial>
  MaterialFactory::CreateMaterial(const nlohmann::json& serializedMaterial)
  {
    return std::make_shared<Material>(
      m_ShaderFactory->CreateShader(serializedMaterial["shader"]),
      m_MaterialPropertiesFactory->CreateMaterialProperties(
        serializedMaterial["materialProperties"]),
      m_ShaderParameterCollectionFactory->CreateShaderParameterCollection(
        serializedMaterial["ShaderParameterCollection"]));
  }
}