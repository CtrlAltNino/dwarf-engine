#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Base.h"
#include <boost/di/extension/injections/lazy.hpp>
#include <memory>

namespace Dwarf
{
  class ShaderSourceCollectionFactory : public IShaderSourceCollectionFactory
  {
  private:
    boost::di::extension::lazy<std::shared_ptr<IAssetDatabase>> mAssetDatabase;
    GraphicsApi                                                 mGraphicsApi;

  public:
    ShaderSourceCollectionFactory(
      boost::di::extension::lazy<std::shared_ptr<IAssetDatabase>> assetDatabase,
      GraphicsApi                                                 graphicsApi);

    ~ShaderSourceCollectionFactory() override = default;

    /**
     * @brief Create a shader source collection containing the shader sources
     * for the default shader
     *
     * @return Unique pointer to the created shader source collection
     */
    auto
    CreateDefaultShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> override;

    /**
     * @brief Create a shader source collection containing the shader sources
     * for the error shader
     *
     * @return Unique pointer to the created shader source collection
     */
    auto
    CreateErrorShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> override;

    /**
     * @brief Create a shader source collection containing the shader sources
     * for the id shader
     *
     * @return Unique pointer to the created shader source collection
     */
    auto
    CreateIdShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> override;

    /**
     * @brief Create a shader source collection containing the shader sources
     * for the grid shader
     *
     * @return Unique pointer to the created shader source collection
     */
    auto
    CreateGridShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> override;

    /**
     * @brief Create a shader source collection containing the shader sources
     * for the agx tonemap shader
     *
     * @return Unique pointer to the created shader source collection
     */
    auto
    CreateAgxTonemapShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> override;

    /**
     * @brief Create a shader source collection containing the shader sources
     * for the model preview shader
     *
     * @return Unique pointer to the created shader source collection
     */
    auto
    CreatePreviewShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> override;

    /**
     * @brief Create a shader source collection containing the shader sources
     * serialized in a json object
     *
     * @return Unique pointer to the created shader source collection
     */
    auto
    CreateShaderSourceCollection(
      const nlohmann::json& serializedShaderSourceCollection)
      -> std::unique_ptr<IShaderSourceCollection> override;

    /**
     * @brief Create a shader source collection containing the shader sources
     * provided
     *
     * @return Unique pointer to the created shader source collection
     */
    auto
    CreateShaderSourceCollection(
      std::vector<std::unique_ptr<IAssetReference>>& shaderSources)
      -> std::unique_ptr<IShaderSourceCollection> override;
  };
}