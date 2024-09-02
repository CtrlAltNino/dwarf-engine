#pragma once

#include "IMaterialFactory.h"
#include "Core/Rendering/Shader/IShaderFactory.h"

namespace Dwarf
{
  class MaterialFactory : public IMaterialFactory
  {
  public:
    // Constructor
    MaterialFactory(std::shared_ptr<IShaderFactory> shaderFactory);

    // Destructor
    ~MaterialFactory();

    // Implement the CreateMaterial method
    virtual std::shared_ptr<IMaterial>
    CreateDefaultMaterial() const override;

    virtual std::shared_ptr<IMaterial>
    CreateMaterial(std::shared_ptr<IShader> shader) const override;

    virtual std::shared_ptr<IMaterial>
    FromSerialized(const nlohmann::json& serializedMaterial) const override;

  private:
    std::shared_ptr<IShaderFactory> m_ShaderFactory;
  };
};