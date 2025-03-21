#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.h"

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
     * for the agx tonemap shader
     *
     * @return Unique pointer to the created shader source collection
     */
    virtual auto
    CreateAgxTonemapShaderSourceCollection()
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
  };
}