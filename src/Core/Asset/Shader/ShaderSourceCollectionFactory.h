#pragma once

#include "Core/Asset/Shader/IShaderSourceCollectionFactory.h"
#include <memory>

namespace Dwarf
{
  class ShaderSourceCollectionFactory : public IShaderSourceCollectionFactory
  {
  private:
    // std::shared_ptr<IAssetDatabase> m_AssetDatabase;

  public:
    ShaderSourceCollectionFactory(
      // std::shared_ptr<IAssetDatabase> assetDatabase
    );
    ~ShaderSourceCollectionFactory() override = default;

    ShaderSourceCollection
    CreateShaderSourceCollection(
      nlohmann::json serializedShaderSourceCollection) override;
  };
}