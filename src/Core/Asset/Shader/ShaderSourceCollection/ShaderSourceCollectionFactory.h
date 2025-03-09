#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Base.h"
#include "Project/IProjectSettings.h"
#include <boost/di/extension/injections/lazy.hpp>
#include <memory>

namespace Dwarf
{
  class ShaderSourceCollectionFactory : public IShaderSourceCollectionFactory
  {
  private:
    boost::di::extension::lazy<std::shared_ptr<IAssetDatabase>> m_AssetDatabase;
    GraphicsApi                                                 m_GraphicsApi;

  public:
    ShaderSourceCollectionFactory(
      boost::di::extension::lazy<std::shared_ptr<IAssetDatabase>> assetDatabase,
      GraphicsApi                                                 graphicsApi);

    ~ShaderSourceCollectionFactory() override = default;

    std::unique_ptr<IShaderSourceCollection>
    CreateDefaultShaderSourceCollection() override;

    std::unique_ptr<IShaderSourceCollection>
    CreateErrorShaderSourceCollection() override;

    std::unique_ptr<IShaderSourceCollection>
    CreateIdShaderSourceCollection() override;

    std::unique_ptr<IShaderSourceCollection>
    CreateGridShaderSourceCollection() override;

    std::unique_ptr<IShaderSourceCollection>
    CreateAgxTonemapShaderSourceCollection() override;

    std::unique_ptr<IShaderSourceCollection>
    CreateShaderSourceCollection(
      const nlohmann::json& serializedShaderSourceCollection) override;
  };
}