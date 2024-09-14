#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Base.h"
#include <memory>

namespace Dwarf
{
  class ShaderSourceCollectionFactory : public IShaderSourceCollectionFactory
  {
  private:
    std::shared_ptr<IAssetDatabase> m_AssetDatabase;
    GraphicsApi                     m_GraphicsApi;

  public:
    ShaderSourceCollectionFactory(std::shared_ptr<IAssetDatabase> assetDatabase,
                                  GraphicsApi                     graphicsApi);
    ~ShaderSourceCollectionFactory() override = default;

    std::unique_ptr<IShaderSourceCollection>
    CreateDefaultShaderSourceCollection() override;

    std::unique_ptr<IShaderSourceCollection>
    CreateShaderSourceCollection(
      const nlohmann::json& serializedShaderSourceCollection) override;
  };
}