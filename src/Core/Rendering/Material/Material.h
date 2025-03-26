#pragma once
#include "Core/Rendering/Shader/IShader.h"
#include "IMaterial.h"
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

  public:
    Material(std::shared_ptr<IShader>                    shader,
             MaterialProperties                          materialProperties,
             std::unique_ptr<IShaderParameterCollection> shaderParameters);
    ~Material() override = default;

    /**
     * @brief Get the shader for this material.
     *
     * @return The shader for this material.
     */
    auto
    GetShader() -> std::shared_ptr<IShader> override;

    void
    SetShader(std::shared_ptr<IShader> shader) override;

    [[nodiscard]] auto
    GetShaderParameters() const
      -> const std::unique_ptr<IShaderParameterCollection>& override;

    auto
    GetMaterialProperties() -> MaterialProperties& override;

    void
    GenerateShaderParameters() override;

    auto
    Serialize() -> nlohmann::json override;
  };
}