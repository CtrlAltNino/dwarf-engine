#pragma once

#include "IMaterialFactory.h"
#include "Core/Rendering/Shader/IShaderFactory.h"
#include "Core/Rendering/Material/IMaterialPropertiesFactory.h"
#include "Core/Rendering/Shader/IShaderParameterCollectionFactory.h"

namespace Dwarf
{
  class MaterialFactory : public IMaterialFactory
  {
  public:
    // Constructor
    MaterialFactory(
      std::shared_ptr<IShaderFactory>             shaderFactory,
      std::shared_ptr<IMaterialPropertiesFactory> materialPropertiesFactory,
      std::shared_ptr<IShaderParameterCollectionFactory>
        shaderParameterCollectionFactory);

    // Destructor
    ~MaterialFactory();

    // Implement the CreateMaterial method
    virtual std::shared_ptr<IMaterial>
    CreateDefaultMaterial() override;

    virtual std::shared_ptr<IMaterial>
    CreateMaterial(std::shared_ptr<IShader> shader) override;

    virtual std::shared_ptr<IMaterial>
    FromSerialized(const nlohmann::json& serializedMaterial) override;

  private:
    std::shared_ptr<IShaderFactory>             m_ShaderFactory;
    std::shared_ptr<IMaterialPropertiesFactory> m_MaterialPropertiesFactory;
    std::shared_ptr<IShaderParameterCollectionFactory>
      m_ShaderParameterCollectionFactory;
  };
};