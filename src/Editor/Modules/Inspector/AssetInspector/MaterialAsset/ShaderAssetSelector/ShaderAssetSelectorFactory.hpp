#pragma once

#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.hpp"
#include "IShaderAssetSelectorFactory.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class ShaderAssetSelectorFactory : public IShaderAssetSelectorFactory
  {
  private:
    GraphicsApi                   mGraphicsApi;
    std::shared_ptr<IDwarfLogger> mLogger;
    std::shared_ptr<IShaderSourceCollectionFactory>
                                    mShaderSourceCollectionFactory;
    std::shared_ptr<IAssetDatabase> mAssetDatabase;

  public:
    ShaderAssetSelectorFactory(GraphicsApi                   graphicsApi,
                               std::shared_ptr<IDwarfLogger> logger,
                               std::shared_ptr<IShaderSourceCollectionFactory>
                                 shaderSourceCollectionFactory,
                               std::shared_ptr<IAssetDatabase> assetDatabase);
    ~ShaderAssetSelectorFactory() override;

    /**
     * @brief Creates a ShaderAssetSelector instance based on the used graphics
     * api
     *
     * @return Unique pointer to the created instance
     */
    auto
    Create() -> std::unique_ptr<IShaderAssetSelector> override;
  };
}