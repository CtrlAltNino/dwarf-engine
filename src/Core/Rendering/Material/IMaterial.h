#pragma once
#include "Core/Rendering/Shader/IShader.h"
#include "Core/Rendering/Shader/IShaderParameterCollection.h"

namespace Dwarf
{
  struct MaterialProperties
  {
    bool IsTransparent;
    bool IsDoubleSided;
    bool IsUnlit;
    bool IsWireframe;
  };

  class IMaterial
  {
  public:
    virtual ~IMaterial() = default;

    virtual const std::shared_ptr<IShader>&
    GetShader() const;

    virtual const std::shared_ptr<IShaderParameterCollection>&
    GetParameters() const;

    virtual MaterialProperties&
    GetProperties();

    virtual nlohmann::json
    Serialize() const = 0;
  };
}