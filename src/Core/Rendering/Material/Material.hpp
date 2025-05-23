#pragma once

#include "Core/Rendering/Shader/IShader.hpp"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.hpp"
#include "IMaterial.hpp"
#include "ShaderAssetSourceContainer/IShaderAssetSourceContainer.hpp"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  /// @brief Class representing a modular Material.
  class Material : public IMaterial
  {
  private:
    /// @brief Material properties.
    MaterialProperties mMaterialProperties;

    /// @brief Shader program for this material.
    std::shared_ptr<IShader> mShader;

    /// @brief Shader parameters for this material.
    std::unique_ptr<IShaderParameterCollection> mShaderParameters;

    std::unique_ptr<IShaderAssetSourceContainer> mShaderAssetSourceContainer;

    std::weak_ptr<IShaderRegistry> mShaderRegistry;

  public:
    Material(
      MaterialProperties                           materialProperties,
      std::unique_ptr<IShaderParameterCollection>  shaderParameters,
      std::unique_ptr<IShaderAssetSourceContainer> shaderAssetSourceContainer,
      const std::shared_ptr<IShaderRegistry>&      shaderRegistry);
    ~Material() override = default;

    /**
     * @brief Get the shader for this material.
     *
     * @return The shader for this material.
     */
    auto
    GetShader() -> std::shared_ptr<IShader> override;

    /**
     * @brief Updates the
     *
     */
    void
    UpdateShader() override;

    /**
     * @brief Gets the ShaderParameterCollection
     *
     * @return Reference to the stored ShaderParameterCollection
     */
    [[nodiscard]] auto
    GetShaderParameters() const
      -> const std::unique_ptr<IShaderParameterCollection>& override;

    /**
     * @brief Gets the properties of the materials
     *
     * @return Reference to the MaterialProperties instance
     */
    auto
    GetMaterialProperties() -> MaterialProperties& override;

    /**
     * @brief Clears the shader parameters and pulls them fresh from the shader
     *
     */
    void
    GenerateShaderParameters() override;

    /**
     * @brief Gets the Shader asset sources
     *
     * @return Reference to the stored ShaderAssetSourceContainer instance
     */
    [[nodiscard]] auto
    GetShaderAssetSources()
      -> std::unique_ptr<IShaderAssetSourceContainer>& override;

    auto
    Serialize() -> nlohmann::json override;
  };
}