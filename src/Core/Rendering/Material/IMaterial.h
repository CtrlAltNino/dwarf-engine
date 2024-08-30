#pragma once
#include "Core/Rendering/Shader/IShader.h"
#include "Core/Rendering/Shader/IShaderParameterCollection.h"
#include "Utilities/ISerializable.h"

namespace Dwarf
{
  struct MaterialProperties : public ISerializable
  {
    bool IsTransparent;
    bool IsDoubleSided;
    bool IsUnlit;
    bool IsWireframe;

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
    GetShader() const;

    virtual void
    SetShader(std::shared_ptr<IShader> shader);

    virtual const std::shared_ptr<IShaderParameterCollection>&
    GetShaderParameters() const;

    virtual MaterialProperties&
    GetMaterialProperties();

    virtual void
    GenerateShaderParameters() = 0;

    virtual nlohmann::json
    Serialize() const = 0;
  };
}