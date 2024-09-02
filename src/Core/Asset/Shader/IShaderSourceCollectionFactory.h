#pragma once

#include "Core/Rendering/Shader/ShaderTypes.h"

namespace Dwarf
{
  class IShaderSourceCollectionFactory
  {
  public:
    virtual ~IShaderSourceCollectionFactory() = default;

    virtual ShaderSourceCollection
    CreateShaderSourceCollection(
      nlohmann::json serializedShaderSourceCollection) = 0;
  };
}