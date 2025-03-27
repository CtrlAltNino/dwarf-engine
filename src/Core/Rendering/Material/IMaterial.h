#pragma once
#include "Core/Rendering/Shader/IShader.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollection.h"
#include "ShaderAssetSourceContainer/IShaderAssetSourceContainer.h"
#include "Utilities/ISerializable.h"

namespace Dwarf
{
  /**
   * @brief A struct that contains all properties of a material with the ability
   * to serialize and deserialize them
   *
   */
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

    auto
    Serialize() -> nlohmann::json override
    {
      return {
        { "IsTransparent", IsTransparent },
        { "IsDoubleSided", IsDoubleSided },
        { "IsUnlit", IsUnlit },
        { "IsWireframe", IsWireframe },
      };
    }
  };

  /**
   * @brief A class that represents a material
   *
   */
  class IMaterial
  {
  public:
    virtual ~IMaterial() = default;

    /**
     * @brief Get the shader for this material.
     *
     * @return The shader for this material.
     */
    virtual auto
    GetShader() -> std::shared_ptr<IShader> = 0;

    virtual void
    UpdateShader() = 0;

    /**
     * @brief Get the shader parameters for this material.
     *
     * @return The shader parameters for this material.
     */
    [[nodiscard]] virtual auto
    GetShaderParameters() const
      -> const std::unique_ptr<IShaderParameterCollection>& = 0;

    /**
     * @brief Get the properties for this material.
     *
     * @return The material properties for this material.
     */
    virtual auto
    GetMaterialProperties() -> MaterialProperties& = 0;

    /**
     * @brief Generate the shader parameters for this material based on the
     * shader. This clears the existing shader parameters.
     */
    virtual void
    GenerateShaderParameters() = 0;

    [[nodiscard]] virtual auto
    GetShaderAssetSources()
      -> std::unique_ptr<IShaderAssetSourceContainer>& = 0;

    /**
     * @brief Serialize the material.
     *
     * @return The serialized material.
     */
    virtual auto
    Serialize() -> nlohmann::json = 0;
  };
}