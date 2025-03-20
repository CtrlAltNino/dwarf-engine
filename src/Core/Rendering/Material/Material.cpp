#include "Material.h"
#include "pch.h"

namespace Dwarf
{
  // std::shared_ptr<Material> Material::s_DefaultMaterial = nullptr;
  // std::shared_ptr<Material> Material::s_ErrorMaterial = nullptr;
  // std::shared_ptr<Material> Material::s_GridMaterial = nullptr;
  // std::shared_ptr<Material> Material::s_PreviewMaterial = nullptr;
  // std::shared_ptr<Material> Material::s_IdMaterial = nullptr;
  // std::shared_ptr<Material> Material::s_WhiteMaterial = nullptr;

  // void
  // Material::Init()
  // {
  //   s_DefaultMaterial =
  //     std::make_shared<Material>("Default Material",
  //     Shader::s_DefaultShader);
  //   s_ErrorMaterial =
  //     std::make_shared<Material>("Error Material", Shader::s_ErrorShader);
  //   s_GridMaterial =
  //     std::make_shared<Material>("grid material", Shader::s_GridShader);
  //   s_PreviewMaterial =
  //     std::make_shared<Material>("preview material",
  //     Shader::s_PreviewShader);
  //   s_IdMaterial =
  //     std::make_shared<Material>("id material", Shader::s_IdShader);
  //   s_IdMaterial->GenerateShaderParameters();
  //   s_WhiteMaterial =
  //     std::make_shared<Material>("outline material", Shader::s_WhiteShader);
  //   s_GridMaterial->SetTransparency(true);
  // }

  // Material::Material(std::string_view name)
  //   : mName(name)
  // {
  //   mShader = Shader::s_DefaultShader;
  // }

  Material::Material(
    std::shared_ptr<IShader>                    shader,
    MaterialProperties                          materialProperties,
    std::unique_ptr<IShaderParameterCollection> shaderParameters)
    : mShader(shader)
    , mMaterialProperties(materialProperties)
    , mShaderParameters(std::move(shaderParameters))
  {
    mShader->Compile();
  }

  Material::~Material() {}

  // void
  // Material::SetShader(std::shared_ptr<IShader> shader)
  // {
  //   mShader = shader;
  // }

  IShader&
  Material::GetShader()
  {
    return *mShader;
  }

  void
  Material::SetShader(std::shared_ptr<IShader> shader)
  {
    mShader = shader;
  }

  const std::unique_ptr<IShaderParameterCollection>&
  Material::GetShaderParameters() const
  {
    return mShaderParameters;
  }

  MaterialProperties&
  Material::GetMaterialProperties()
  {
    return mMaterialProperties;
  }

  void
  Material::GenerateShaderParameters()
  {
    mShaderParameters = std::move(mShader->CreateParameters());
  }

  nlohmann::json
  Material::Serialize()
  {
    nlohmann::json serializedMaterial;
    serializedMaterial["Shader"] = mShader->Serialize();
    serializedMaterial["Properties"] = mMaterialProperties.Serialize();
    serializedMaterial["ShaderParameters"] = mShaderParameters->Serialize();

    return serializedMaterial;
  }

  // void
  // Material::SetTransparency(bool transparent)
  // {
  //   mTransparent = transparent;
  // }

  // bool
  // Material::IsTransparent() const
  // {
  //   return mTransparent;
  // }

  // void
  // Material::GenerateShaderParameters()
  // {
  //   // Get all shader inputs from abstract Shader function and put them in
  //   the
  //   // maps
  //   mParameters = mShader->GetParameters();
  // }

  // template<>
  // void
  // Material::SetParameter<bool>(std::string_view identifier, bool value)
  // {
  //   if (mParameters.contains(identifier) &&
  //       (mParameters[std::string(identifier)]->GetType() ==
  //        ShaderParameterType::BOOLEAN))
  //   {
  //     std::dynamic_pointer_cast<BooleanShaderParameter>(
  //       mParameters[std::string(identifier)])
  //       ->mValue = value;
  //   }
  //   else
  //   {
  //     mParameters[std::string(identifier)] =
  //       std::make_shared<BooleanShaderParameter>(value);
  //   }
  // }

  // template<>
  // void
  // Material::SetParameter<int>(std::string_view identifier, int value)
  // {
  //   if (mParameters.contains(identifier) &&
  //       (mParameters[std::string(identifier)]->GetType() ==
  //        ShaderParameterType::INTEGER))
  //   {
  //     std::dynamic_pointer_cast<IntegerShaderParameter>(
  //       mParameters[std::string(identifier)])
  //       ->mValue = value;
  //   }
  //   else
  //   {
  //     mParameters[std::string(identifier)] =
  //       std::make_shared<IntegerShaderParameter>(value);
  //   }
  // }

  // template<>
  // void
  // Material::SetParameter<unsigned int>(std::string_view identifier,
  //                                      unsigned int     value)
  // {
  //   if (mParameters.contains(identifier) &&
  //       (mParameters[std::string(identifier)]->GetType() ==
  //        ShaderParameterType::UNSIGNED_INTEGER))
  //   {
  //     std::dynamic_pointer_cast<UnsignedIntegerShaderParameter>(
  //       mParameters[std::string(identifier)])
  //       ->mValue = value;
  //   }
  //   else
  //   {
  //     mParameters[std::string(identifier)] =
  //       std::make_shared<UnsignedIntegerShaderParameter>(value);
  //   }
  // }

  // template<>
  // void
  // Material::SetParameter<float>(std::string_view identifier, float value)
  // {
  //   if (mParameters.contains(identifier) &&
  //       (mParameters.find(identifier)->second->GetType() ==
  //        ShaderParameterType::FLOAT))
  //   {
  //     std::dynamic_pointer_cast<FloatShaderParameter>(
  //       mParameters[std::string(identifier)])
  //       ->mValue = value;
  //   }
  //   else
  //   {
  //     mParameters[std::string(identifier)] =
  //       std::make_shared<FloatShaderParameter>(value);
  //   }
  // }

  // template<>
  // void
  // Material::SetParameter<glm::vec2>(std::string_view identifier,
  //                                   glm::vec2        value)
  // {
  //   if (mParameters.contains(identifier) &&
  //       (mParameters[std::string(identifier)]->GetType() ==
  //        ShaderParameterType::VEC2))
  //   {
  //     std::dynamic_pointer_cast<Vec2ShaderParameter>(
  //       mParameters[std::string(identifier)])
  //       ->mValue = value;
  //   }
  //   else
  //   {
  //     mParameters[std::string(identifier)] =
  //       std::make_shared<Vec2ShaderParameter>(value);
  //   }
  // }

  // template<>
  // void
  // Material::SetParameter<glm::vec3>(std::string_view identifier,
  //                                   glm::vec3        value)
  // {
  //   if (mParameters.contains(identifier) &&
  //       (mParameters[std::string(identifier)]->GetType() ==
  //        ShaderParameterType::VEC3))
  //   {
  //     std::dynamic_pointer_cast<Vec3ShaderParameter>(
  //       mParameters[std::string(identifier)])
  //       ->mValue = value;
  //   }
  //   else
  //   {
  //     mParameters[std::string(identifier)] =
  //       std::make_shared<Vec3ShaderParameter>(value);
  //   }
  // }

  // template<>
  // void
  // Material::SetParameter<glm::vec4>(std::string_view identifier,
  //                                   glm::vec4        value)
  // {
  //   if (mParameters.contains(identifier) &&
  //       (mParameters[std::string(identifier)]->GetType() ==
  //        ShaderParameterType::VEC4))
  //   {
  //     std::dynamic_pointer_cast<Vec4ShaderParameter>(
  //       mParameters[std::string(identifier)])
  //       ->mValue = value;
  //   }
  //   else
  //   {
  //     mParameters[std::string(identifier)] =
  //       std::make_shared<Vec4ShaderParameter>(value);
  //   }
  // }

  // void
  // Material::SetParameter(std::string_view     identifier,
  //                        std::shared_ptr<UUID> value,
  //                        ShaderParameterType  type)
  // {
  //   if (mParameters.contains(identifier) &&
  //       (mParameters[std::string(identifier)]->GetType() == type))
  //   {
  //     switch (type)
  //     {
  //       using enum ShaderParameterType;
  //       case TEX2D:
  //         std::dynamic_pointer_cast<Tex2DShaderParameter>(
  //           mParameters[std::string(identifier)])
  //           ->mValue = value;
  //         break;
  //       case BOOLEAN: break;
  //       case INTEGER: break;
  //       case UNSIGNED_INTEGER: break;
  //       case FLOAT: break;
  //       case VEC2: break;
  //       case VEC3: break;
  //       case VEC4: break;
  //     }
  //   }
  //   else
  //   {
  //     switch (type)
  //     {
  //       using enum ShaderParameterType;
  //       case TEX2D:
  //         mParameters[std::string(identifier)] =
  //           std::make_shared<Tex2DShaderParameter>(value);
  //         break;
  //       case BOOLEAN: break;
  //       case INTEGER: break;
  //       case UNSIGNED_INTEGER: break;
  //       case FLOAT: break;
  //       case VEC2: break;
  //       case VEC3: break;
  //       case VEC4: break;
  //     }
  //   }
  // }
}