#pragma once

#include <glm/glm.hpp>
#include <map>
#include <string_view>

#include "Core/Base.h"
#include "Core/Rendering/Shader.h"
#include "Core/Rendering/Texture.h"
#include "Core/Rendering/IShaderParameter.h"
#include "Core/UID.h"

namespace Dwarf
{

  /// @brief Class representing a modular Material.
  class Material
  {
  public:
    explicit Material(std::string_view name);
    Material(std::string_view name, std::shared_ptr<Shader> shader);
    ~Material();

    static std::shared_ptr<Material> s_DefaultMaterial;
    static std::shared_ptr<Material> s_ErrorMaterial;
    static std::shared_ptr<Material> s_GridMaterial;
    static std::shared_ptr<Material> s_PreviewMaterial;
    static std::shared_ptr<Material> s_IdMaterial;
    static std::shared_ptr<Material> s_WhiteMaterial;
    static void
    Init();

    /// @brief Name of the material.
    std::string m_Name;

    /// @brief Shader program for this material.
    std::shared_ptr<Shader> m_Shader;

    /// @brief Flag indicating if this is a transparent object.
    bool m_Transparent;

    std::map<std::string, std::shared_ptr<IShaderParameter>, std::less<>>
      m_Parameters = {};

    std::string
    GetName() const;

    std::shared_ptr<Shader> const&
    GetShader() const;

    void
    SetShader(std::shared_ptr<Shader> shader);

    template<typename T>
    void
    SetParameter(std::string_view identifier, T parameter);

    void
    SetParameter(std::string_view     identifier,
                 std::shared_ptr<UID> value,
                 ShaderParameterType  type);

    void
    SetTransparency(bool transparent);

    bool
    IsTransparent() const;

    void
    GenerateShaderParameters();
  };
}