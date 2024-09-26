#pragma once
#include "Core/Rendering/Shader/IShader.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollection.h"
#include "Utilities/ISerializable.h"

namespace Dwarf
{
  struct MaterialProperties : public ISerializable
  {
    bool IsTransparent;
    bool IsDoubleSided;
    bool IsUnlit;
    bool IsWireframe;

    MaterialProperties(bool isTransparent = false,
                       bool isDoubleSided = false,
                       bool isUnlit = false,
                       bool isWireframe = false)
      : IsTransparent(isTransparent)
      , IsDoubleSided(isDoubleSided)
      , IsUnlit(isUnlit)
      , IsWireframe(isWireframe)
    {
    }

    MaterialProperties(nlohmann::json serializedMaterialProperties)
      : IsTransparent(serializedMaterialProperties["IsTransparent"])
      , IsDoubleSided(serializedMaterialProperties["IsDoubleSided"])
      , IsUnlit(serializedMaterialProperties["IsUnlit"])
      , IsWireframe(serializedMaterialProperties["IsWireframe"])
    {
    }

    nlohmann::json
    Serialize() const override
    {
      return {
        { "IsTransparent", IsTransparent },
        { "IsDoubleSided", IsDoubleSided },
        { "IsUnlit", IsUnlit },
        { "IsWireframe", IsWireframe },
      };
    }
  };

  class IMaterial
  {
  public:
    virtual ~IMaterial() = default;

    /**
     * @brief Get the shader for this material.
     *
     * @return The shader for this material.
     */
    virtual IShader&
    GetShader() = 0;

    /**
     * @brief Set the shader for this material.
     *
     * @param shader The shader to set.
     */
    virtual void
    SetShader(std::unique_ptr<IShader>&& shader) = 0;

    /**
     * @brief Get the shader parameters for this material.
     *
     * @return The shader parameters for this material.
     */
    virtual const std::unique_ptr<IShaderParameterCollection>&
    GetShaderParameters() const = 0;

    /**
     * @brief Get the properties for this material.
     *
     * @return The material properties for this material.
     */
    virtual MaterialProperties&
    GetMaterialProperties() = 0;

    /**
     * @brief Generate the shader parameters for this material based on the
     * shader. This clears the existing shader parameters.
     */
    virtual void
    GenerateShaderParameters() = 0;

    /**
     * @brief Serialize the material.
     *
     * @return The serialized material.
     */
    virtual nlohmann::json
    Serialize() const = 0;
  };
}