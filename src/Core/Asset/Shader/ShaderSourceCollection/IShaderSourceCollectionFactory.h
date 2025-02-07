#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.h"

namespace Dwarf
{
  class IShaderSourceCollectionFactory
  {
  public:
    virtual ~IShaderSourceCollectionFactory() = default;

    virtual std::unique_ptr<IShaderSourceCollection>
    CreateDefaultShaderSourceCollection() = 0;

    virtual std::unique_ptr<IShaderSourceCollection>
    CreateErrorShaderSourceCollection() = 0;

    virtual std::unique_ptr<IShaderSourceCollection>
    CreateIdShaderSourceCollection() = 0;

    virtual std::unique_ptr<IShaderSourceCollection>
    CreateGridShaderSourceCollection() = 0;

    virtual std::unique_ptr<IShaderSourceCollection>
    CreateShaderSourceCollection(
      const nlohmann::json& serializedShaderSourceCollection) = 0;
  };
}