#include "dpch.h"
#include "Core/Rendering/Material.h"
#include "Core/Rendering/IShaderParameter.h"
#include "Core/Rendering/Shader Parameters/BooleanShaderParameter.h"
#include "Core/Rendering/Shader Parameters/FloatShaderParameter.h"
#include "Core/Rendering/Shader Parameters/IntegerShaderParameter.h"
#include "Core/Rendering/Shader Parameters/UnsignedIntegerShaderParameter.h"
#include "Core/Rendering/Shader Parameters/Tex2DShaderParameter.h"
#include "Core/Rendering/Shader Parameters/Vec2ShaderParameter.h"
#include "Core/Rendering/Shader Parameters/Vec3ShaderParameter.h"
#include "Core/Rendering/Shader Parameters/Vec4ShaderParameter.h"

namespace Dwarf
{
  std::shared_ptr<Material> Material::s_DefaultMaterial = nullptr;
  std::shared_ptr<Material> Material::s_ErrorMaterial = nullptr;
  std::shared_ptr<Material> Material::s_GridMaterial = nullptr;
  std::shared_ptr<Material> Material::s_PreviewMaterial = nullptr;
  std::shared_ptr<Material> Material::s_IdMaterial = nullptr;
  std::shared_ptr<Material> Material::s_WhiteMaterial = nullptr;

  void
  Material::Init()
  {
    s_DefaultMaterial =
      std::make_shared<Material>("Default Material", Shader::s_DefaultShader);
    s_ErrorMaterial =
      std::make_shared<Material>("Error Material", Shader::s_ErrorShader);
    s_GridMaterial =
      std::make_shared<Material>("grid material", Shader::s_GridShader);
    s_PreviewMaterial =
      std::make_shared<Material>("preview material", Shader::s_PreviewShader);
    s_IdMaterial =
      std::make_shared<Material>("id material", Shader::s_IdShader);
    s_IdMaterial->GenerateShaderParameters();
    s_WhiteMaterial =
      std::make_shared<Material>("outline material", Shader::s_WhiteShader);
    s_GridMaterial->SetTransparency(true);
  }

  Material::Material(std::string_view name)
    : m_Name(name)
  {
    m_Shader = Shader::s_DefaultShader;
  }

  Material::Material(std::string_view name, std::shared_ptr<Shader> shader)
    : m_Name(name)
    , m_Shader(shader)
  {
  }

  Material::~Material() = default;

  std::string
  Material::GetName() const
  {
    return m_Name;
  }

  void
  Material::SetShader(std::shared_ptr<Shader> shader)
  {
    m_Shader = shader;
  }

  std::shared_ptr<Shader> const&
  Material::GetShader() const
  {
    return m_Shader;
  }

  void
  Material::SetTransparency(bool transparent)
  {
    m_Transparent = transparent;
  }

  bool
  Material::IsTransparent() const
  {
    return m_Transparent;
  }

  void
  Material::GenerateShaderParameters()
  {
    // Get all shader inputs from abstract Shader function and put them in the
    // maps
    m_Parameters = m_Shader->GetParameters();
  }

  template<>
  void
  Material::SetParameter<bool>(std::string_view identifier, bool value)
  {
    if (m_Parameters.contains(identifier) &&
        (m_Parameters[std::string(identifier)]->GetType() ==
         ShaderParameterType::BOOLEAN))
    {
      std::dynamic_pointer_cast<BooleanShaderParameter>(
        m_Parameters[std::string(identifier)])
        ->m_Value = value;
    }
    else
    {
      m_Parameters[std::string(identifier)] =
        std::make_shared<BooleanShaderParameter>(value);
    }
  }

  template<>
  void
  Material::SetParameter<int>(std::string_view identifier, int value)
  {
    if (m_Parameters.contains(identifier) &&
        (m_Parameters[std::string(identifier)]->GetType() ==
         ShaderParameterType::INTEGER))
    {
      std::dynamic_pointer_cast<IntegerShaderParameter>(
        m_Parameters[std::string(identifier)])
        ->m_Value = value;
    }
    else
    {
      m_Parameters[std::string(identifier)] =
        std::make_shared<IntegerShaderParameter>(value);
    }
  }

  template<>
  void
  Material::SetParameter<unsigned int>(std::string_view identifier,
                                       unsigned int     value)
  {
    if (m_Parameters.contains(identifier) &&
        (m_Parameters[std::string(identifier)]->GetType() ==
         ShaderParameterType::UNSIGNED_INTEGER))
    {
      std::dynamic_pointer_cast<UnsignedIntegerShaderParameter>(
        m_Parameters[std::string(identifier)])
        ->m_Value = value;
    }
    else
    {
      m_Parameters[std::string(identifier)] =
        std::make_shared<UnsignedIntegerShaderParameter>(value);
    }
  }

  template<>
  void
  Material::SetParameter<float>(std::string_view identifier, float value)
  {
    if (m_Parameters.contains(identifier) &&
        (m_Parameters.find(identifier)->second->GetType() ==
         ShaderParameterType::FLOAT))
    {
      std::dynamic_pointer_cast<FloatShaderParameter>(
        m_Parameters[std::string(identifier)])
        ->m_Value = value;
    }
    else
    {
      m_Parameters[std::string(identifier)] =
        std::make_shared<FloatShaderParameter>(value);
    }
  }

  template<>
  void
  Material::SetParameter<glm::vec2>(std::string_view identifier,
                                    glm::vec2        value)
  {
    if (m_Parameters.contains(identifier) &&
        (m_Parameters[std::string(identifier)]->GetType() ==
         ShaderParameterType::VEC2))
    {
      std::dynamic_pointer_cast<Vec2ShaderParameter>(
        m_Parameters[std::string(identifier)])
        ->m_Value = value;
    }
    else
    {
      m_Parameters[std::string(identifier)] =
        std::make_shared<Vec2ShaderParameter>(value);
    }
  }

  template<>
  void
  Material::SetParameter<glm::vec3>(std::string_view identifier,
                                    glm::vec3        value)
  {
    if (m_Parameters.contains(identifier) &&
        (m_Parameters[std::string(identifier)]->GetType() ==
         ShaderParameterType::VEC3))
    {
      std::dynamic_pointer_cast<Vec3ShaderParameter>(
        m_Parameters[std::string(identifier)])
        ->m_Value = value;
    }
    else
    {
      m_Parameters[std::string(identifier)] =
        std::make_shared<Vec3ShaderParameter>(value);
    }
  }

  template<>
  void
  Material::SetParameter<glm::vec4>(std::string_view identifier,
                                    glm::vec4        value)
  {
    if (m_Parameters.contains(identifier) &&
        (m_Parameters[std::string(identifier)]->GetType() ==
         ShaderParameterType::VEC4))
    {
      std::dynamic_pointer_cast<Vec4ShaderParameter>(
        m_Parameters[std::string(identifier)])
        ->m_Value = value;
    }
    else
    {
      m_Parameters[std::string(identifier)] =
        std::make_shared<Vec4ShaderParameter>(value);
    }
  }

  void
  Material::SetParameter(std::string_view     identifier,
                         std::shared_ptr<UID> value,
                         ShaderParameterType  type)
  {
    if (m_Parameters.contains(identifier) &&
        (m_Parameters[std::string(identifier)]->GetType() == type))
    {
      switch (type)
      {
        using enum ShaderParameterType;
        case TEX2D:
          std::dynamic_pointer_cast<Tex2DShaderParameter>(
            m_Parameters[std::string(identifier)])
            ->m_Value = value;
          break;
        case BOOLEAN: break;
        case INTEGER: break;
        case UNSIGNED_INTEGER: break;
        case FLOAT: break;
        case VEC2: break;
        case VEC3: break;
        case VEC4: break;
      }
    }
    else
    {
      switch (type)
      {
        using enum ShaderParameterType;
        case TEX2D:
          m_Parameters[std::string(identifier)] =
            std::make_shared<Tex2DShaderParameter>(value);
          break;
        case BOOLEAN: break;
        case INTEGER: break;
        case UNSIGNED_INTEGER: break;
        case FLOAT: break;
        case VEC2: break;
        case VEC3: break;
        case VEC4: break;
      }
    }
  }
}