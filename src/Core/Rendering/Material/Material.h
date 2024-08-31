#pragma once
#include "Core/Rendering/Shader/IShaderParameterCollectionFactory.h"
#include "pch.h"
#include "Core/Base.h"
#include "IMaterial.h"
#include "Core/Rendering/Shader/IShader.h"
#include "Core/UUID.h"
#include "IMaterial.h"

namespace Dwarf
{
  /// @brief Class representing a modular Material.
  class Material : public IMaterial
  {
  public:
    explicit Material(std::shared_ptr<IShader> shader = nullptr,
                      const nlohmann::json& serializedMaterialProperties = "");
    ~Material() override{};

    // static std::shared_ptr<Material> s_DefaultMaterial;
    // static std::shared_ptr<Material> s_ErrorMaterial;
    // static std::shared_ptr<Material> s_GridMaterial;
    // static std::shared_ptr<Material> s_PreviewMaterial;
    // static std::shared_ptr<Material> s_IdMaterial;
    // static std::shared_ptr<Material> s_WhiteMaterial;
    // static void
    // Init();

    /// @brief Name of the material.
    // std::string m_Name;

    /// @brief Flag indicating if this is a transparent object.
    // bool m_Transparent;

    // std::map<std::string, std::shared_ptr<IShaderParameter>, std::less<>>
    //   m_Parameters = {};

    // std::string
    // GetName() const;

    const std::shared_ptr<IShader>
    GetShader() const override;

    void
    SetShader(std::shared_ptr<IShader> shader) override;

    const std::shared_ptr<IShaderParameterCollection>&
    GetShaderParameters() const override;

    MaterialProperties&
    GetMaterialProperties() override;

    void
    GenerateShaderParameters() override;

    nlohmann::json
    Serialize() const override;

  private:
    MaterialProperties m_MaterialProperties;
    std::shared_ptr<IShaderParameterCollectionFactory>
      m_ShaderParameterCollectionFactory;
    std::shared_ptr<IShaderParameterCollection> m_ShaderParameters;
    /// @brief Shader program for this material.
    std::shared_ptr<IShader> m_Shader;

    // void
    // SetShader(std::shared_ptr<IShader> shader);

    // template<typename T>
    // void
    // SetParameter(std::string_view identifier, T parameter);

    // void
    // SetParameter(std::string_view     identifier,
    //              std::shared_ptr<UUID> value,
    //              ShaderParameterType  type);

    // void
    // SetTransparency(bool transparent);

    // bool
    // IsTransparent() const;

    // void
    // GenerateShaderParameters();
  };
}