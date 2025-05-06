#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.hpp"

namespace Dwarf
{
  class IShaderSourceCollectionFactory
  {
  public:
    virtual ~IShaderSourceCollectionFactory() = default;

    /**
     * @brief Create a shader source collection containing the shader sources
     * for the default shader
     *
     * @return Unique pointer to the created shader source collection
     */
    virtual auto
    CreateDefaultShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> = 0;

    /**
     * @brief Create a shader source collection containing the shader sources
     * for the error shader
     *
     * @return Unique pointer to the created shader source collection
     */
    virtual auto
    CreateErrorShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> = 0;

    /**
     * @brief Create a shader source collection containing the shader sources
     * for the id shader
     *
     * @return Unique pointer to the created shader source collection
     */
    virtual auto
    CreateIdShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> = 0;

    /**
     * @brief Create a shader source collection containing the shader sources
     * for the grid shader
     *
     * @return Unique pointer to the created shader source collection
     */
    virtual auto
    CreateGridShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> = 0;

    /**
     * @brief Create a shader source collection containing the shader sources
     * for the reinhard tonemap shader
     *
     * @return Unique pointer to the created shader source collection
     */
    virtual auto
    CreateReinhardTonemapShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> = 0;

    /**
     * @brief Create a shader source collection containing the shader sources
     * for the agx tonemap shader
     *
     * @return Unique pointer to the created shader source collection
     */
    virtual auto
    CreateAgxTonemapShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> = 0;

    /**
     * @brief Create a shader source collection containing the shader sources
     * for the aces tonemap shader
     *
     * @return Unique pointer to the created shader source collection
     */
    virtual auto
    CreateAcesTonemapShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> = 0;

    /**
     * @brief Create a shader source collection containing the shader sources
     * for the Uncharted 2 tonemap shader
     *
     * @return Unique pointer to the created shader source collection
     */
    virtual auto
    CreateUncharted2TonemapShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> = 0;

    /**
     * @brief Create a shader source collection containing the shader sources
     * for the model preview shader
     *
     * @return Unique pointer to the created shader source collection
     */
    virtual auto
    CreatePreviewShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> = 0;

    /**
     * @brief Create a shader source collection containing the shader sources
     * for the pbr shader
     *
     * @return Unique pointer to the created shader source collection
     */
    virtual auto
    CreatePbrShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> = 0;

    /**
     * @brief Create a shader source collection containing the shader sources
     * for the unlit shader
     *
     * @return Unique pointer to the created shader source collection
     */
    virtual auto
    CreateUnlitShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> = 0;

    virtual auto
    CreateColorSkyboxShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> = 0;

    virtual auto
    CreateHdriSkyboxShaderSourceCollection()
      -> std::unique_ptr<IShaderSourceCollection> = 0;

    /**
     * @brief Create a shader source collection containing the shader sources
     * serialized in a json object
     *
     * @return Unique pointer to the created shader source collection
     */
    virtual auto
    CreateShaderSourceCollection(
      const nlohmann::json& serializedShaderSourceCollection)
      -> std::unique_ptr<IShaderSourceCollection> = 0;

    /**
     * @brief Create a shader source collection containing the shader sources
     * provided
     *
     * @return Unique pointer to the created shader source collection
     */
    virtual auto
    CreateShaderSourceCollection(
      std::vector<std::unique_ptr<IAssetReference>>& shaderSources)
      -> std::unique_ptr<IShaderSourceCollection> = 0;
  };
}