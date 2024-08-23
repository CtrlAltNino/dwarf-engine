#pragma once
#include "Core/Rendering/Shader/IShader.h"
#include "Core/Rendering/Shader/IShaderParameterCollection.h"
#include "Core/Rendering/Material/Properties/IMaterialProperties.h"

namespace Dwarf
{
  class IMaterial
  {
  public:
    virtual ~IMaterial() = default;

    virtual const std::shared_ptr<IShader>&
    GetShader() const;

    virtual const std::shared_ptr<IShaderParameterCollection>&
    GetParameters() const;

    virtual const std::shared_ptr<IMaterialProperties>&
    GetProperties() const;

    virtual nlohmann::json
    Serialize() const = 0;
  };
}