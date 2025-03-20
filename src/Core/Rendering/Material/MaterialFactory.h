#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.h"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.h"
#include "IMaterialFactory.h"
#include "Logging/IDwarfLogger.h"
#include <boost/di.hpp>

namespace Dwarf
{
  class MaterialFactory : public IMaterialFactory
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    std::shared_ptr<IShaderParameterCollectionFactory>
                                     mShaderParameterCollectionFactory;
    std::shared_ptr<IShaderRegistry> mShaderRegistry;
    std::shared_ptr<IShaderSourceCollectionFactory>
      mShaderSourceCollectionFactory;

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