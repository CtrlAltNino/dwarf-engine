#pragma once

#include "IShaderParameterCollectionFactory.h"

namespace Dwarf
{
  class ShaderParameterCollectionFactory
    : public IShaderParameterCollectionFactory
  {
  public:
    std::shared_ptr<IShaderParameterCollection>
    CreateShaderParameterCollection() override;

    std::shared_ptr<IShaderParameterCollection>
    CreateShaderParameterCollection(
      const nlohmann::json& serializedShaderParameterCollection) override;
  };
} // namespace Dwarf