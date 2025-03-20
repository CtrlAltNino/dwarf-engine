#pragma once
#include "Core/Base.h"
#include "Core/Rendering/Shader/IShader.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.h"
#include "Core/UUID.h"
#include "IMaterial.h"
#include "pch.h"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  BOOST_STRONG_TYPEDEF(nlohmann::json, SerializedMaterial);
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
    ~Material() override;

    /**
     * @brief Get the shader for this material.
     *
     * @return The shader for this material.
     */
    IShader&
    GetShader() override;

    void
    SetShader(std::shared_ptr<IShader> shader) override;

    const std::unique_ptr<IShaderParameterCollection>&
    GetShaderParameters() const override;

    MaterialProperties&
    GetMaterialProperties() override;

    void
    GenerateShaderParameters() override;

    nlohmann::json
    Serialize() override;
  };
}