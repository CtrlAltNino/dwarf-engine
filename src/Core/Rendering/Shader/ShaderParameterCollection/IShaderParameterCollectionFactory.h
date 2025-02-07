#pragma once
#include "Core/Rendering/Shader/IShader.h"
#include "IShaderParameterCollection.h"

namespace Dwarf
{
  class IShaderParameterCollectionFactory
  {
  public:
    virtual ~IShaderParameterCollectionFactory() = default;

    virtual std::unique_ptr<IShaderParameterCollection>
    CreateShaderParameterCollection() = 0;

    virtual std::unique_ptr<IShaderParameterCollection>
    CreateShaderParameterCollection(
      const nlohmann::json& serializedShaderParameterCollection) = 0;
  };
} // namespace Dwarf