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

    virtual const std::shared_ptr<IShader>
    GetShader() const = 0;

    virtual void
    SetShader(std::shared_ptr<IShader> shader) = 0;

    virtual const std::shared_ptr<IShaderParameterCollection>&
    GetShaderParameters() const = 0;

    virtual MaterialProperties&
    GetMaterialProperties() = 0;

    virtual void
    GenerateShaderParameters() = 0;

    virtual nlohmann::json
    Serialize() const = 0;
  };
}