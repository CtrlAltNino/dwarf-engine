#pragma once

#include "IMaterialFactory.h"
#include "Core/Rendering/Shader/IShaderFactory.h"
#include "Logging/IDwarfLogger.h"
#include <boost/di.hpp>

namespace Dwarf
{
  class MaterialFactory : public IMaterialFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>   m_Logger;
    std::shared_ptr<IShaderFactory> m_ShaderFactory;

  public:
    // Constructor
    BOOST_DI_INJECT(MaterialFactory,
                    std::shared_ptr<IDwarfLogger>   logger,
                    std::shared_ptr<IShaderFactory> shaderFactory);

    // Destructor
    ~MaterialFactory();

    // Implement the CreateMaterial method
    virtual std::unique_ptr<IMaterial>
    CreateDefaultMaterial() const override;

    virtual std::unique_ptr<IMaterial>
    CreateMaterial(std::shared_ptr<IShader> shader) const override;

    virtual std::unique_ptr<IMaterial>
    FromSerialized(const nlohmann::json& serializedMaterial) const override;
  };
};