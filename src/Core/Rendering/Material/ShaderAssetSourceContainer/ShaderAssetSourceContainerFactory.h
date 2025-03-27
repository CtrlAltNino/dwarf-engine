#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Base.h"
#include "IShaderAssetSourceContainerFactory.h"
#include "Logging/IDwarfLogger.h"

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