#pragma once

#include "Core/Base.h"
#include "IShaderParameterCollectionFactory.h"

namespace Dwarf
{
  class ShaderParameterCollectionFactory
    : public IShaderParameterCollectionFactory
  {
  private:
    GraphicsApi mGraphicsApi;

  public:
    ShaderParameterCollectionFactory(GraphicsApi graphicsApi);

    std::unique_ptr<IShaderParameterCollection>
    CreateShaderParameterCollection() override;

    std::unique_ptr<IShaderParameterCollection>
    CreateShaderParameterCollection(
      const nlohmann::json& serializedShaderParameterCollection) override;
  };
} // namespace Dwarf