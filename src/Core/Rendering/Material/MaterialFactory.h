#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.h"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.h"
#include "IMaterialFactory.h"
#include "Logging/IDwarfLogger.h"
#include "ShaderAssetSourceContainer/IShaderAssetSourceContainerFactory.h"
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
    std::shared_ptr<IShaderAssetSourceContainerFactory>
      mShaderAssetSourceContainerFactory;

  public:
    // Constructor
    MaterialFactory(std::shared_ptr<IDwarfLogger> logger,
                    std::shared_ptr<IShaderParameterCollectionFactory>
                      shaderParameterCollectionFactory,
                    std::shared_ptr<IShaderRegistry> shaderRegistry,
                    std::shared_ptr<IShaderSourceCollectionFactory>
                      shaderSourceCollectionFactory,
                    std::shared_ptr<IShaderAssetSourceContainerFactory>
                      shaderAssetSourceContainerFactory);

    // Destructor
    ~MaterialFactory() override;

    /**
     * @brief Creates a default material
     *
     * @return Unique pointer to the created default material
     */
    [[nodiscard]] auto
    CreateDefaultMaterial() const -> std::unique_ptr<IMaterial> override;

    /**
     * @brief Creates a new material which uses the provided shader
     *
     * @param shader Shader to use in the material
     * @return Unique pointer to the created material
     */
    [[nodiscard]] auto
    CreateMaterial(std::unique_ptr<IShaderSourceCollection> shaderSources) const
      -> std::unique_ptr<IMaterial> override;

    /**
     * @brief Creates a material instance based on serialized material data
     *
     * @param serializedMaterial Serialized material data
     * @return Unique pointer to the deserialized material
     */
    [[nodiscard]] auto
    FromSerialized(const nlohmann::json& serializedMaterial) const
      -> std::unique_ptr<IMaterial> override;
  };
};