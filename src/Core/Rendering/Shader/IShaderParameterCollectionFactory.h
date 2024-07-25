#pragma once
#include "IShaderParameterCollection.h"

namespace Dwarf
{
  class IShaderParameterCollectionFactory
  {
  public:
    virtual ~IShaderParameterCollectionFactory() = default;

    virtual std::shared_ptr<IShaderParameterCollection>
    CreateShaderParameterCollection() = 0;

    virtual std::shared_ptr<IShaderParameterCollection>
    CreateShaderParameterCollection(
      const nlohmann::json& serializedShaderParameterCollection) = 0;
  };
} // namespace Dwarf