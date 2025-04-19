#pragma once

#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.hpp"
#include "Core/Base.hpp"
#include "IShaderAssetSourceContainerFactory.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class ShaderAssetSourceContainerFactory
    : public IShaderAssetSourceContainerFactory
  {
  private:
    GraphicsApi                                                 mGraphicsApi;
    std::shared_ptr<IDwarfLogger>                               mLogger;
    boost::di::extension::lazy<std::shared_ptr<IAssetDatabase>> mAssetDatabase;
    std::shared_ptr<IShaderSourceCollectionFactory>
      mShaderSourceCollectionFactory;

  public:
    ShaderAssetSourceContainerFactory(
      GraphicsApi                                                 api,
      std::shared_ptr<IDwarfLogger>                               logger,
      boost::di::extension::lazy<std::shared_ptr<IAssetDatabase>> assetDatabase,
      std::shared_ptr<IShaderSourceCollectionFactory>
        shaderSourceCollectionFactory);
    ~ShaderAssetSourceContainerFactory() override;

    [[nodiscard]] auto
    Create(std::unique_ptr<IShaderSourceCollection> shaderSources) const
      -> std::unique_ptr<IShaderAssetSourceContainer> override;
  };
}