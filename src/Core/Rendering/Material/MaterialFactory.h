#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.h"
#include "IMaterialFactory.h"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.h"
#include "Logging/IDwarfLogger.h"
#include <boost/di.hpp>

namespace Dwarf
{
  class MaterialFactory : public IMaterialFactory
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;
    std::shared_ptr<IShaderParameterCollectionFactory>
                                     m_ShaderParameterCollectionFactory;
    std::shared_ptr<IShaderRegistry> m_ShaderRegistry;
    std::shared_ptr<IShaderSourceCollectionFactory>
      m_ShaderSourceCollectionFactory;

  public:
    // Constructor
    BOOST_DI_INJECT(MaterialFactory,
                    std::shared_ptr<IDwarfLogger> logger,
                    std::shared_ptr<IShaderParameterCollectionFactory>
                      shaderParameterCollectionFactory,
                    std::shared_ptr<IShaderRegistry> shaderRegistry,
                    std::shared_ptr<IShaderSourceCollectionFactory>
                      shaderSourceCollectionFactory);

    // Destructor
    ~MaterialFactory() override;

    // Implement the CreateMaterial method
    virtual std::unique_ptr<IMaterial>
    CreateDefaultMaterial() const override;

    virtual std::unique_ptr<IMaterial>
    CreateMaterial(std::shared_ptr<IShader> shader) const override;

    virtual std::unique_ptr<IMaterial>
    FromSerialized(const nlohmann::json& serializedMaterial) const override;
  };
};