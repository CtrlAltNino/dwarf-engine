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

    /**
     * @brief Creates an empty ShaderParameterCollection
     *
     * @return Unique pointer to the created ShaderParameterCollection
     */
    [[nodiscard]] auto
    Create() const -> std::unique_ptr<IShaderParameterCollection> override;

    /**
     * @brief Creates a ShaderParameterCollection from a serialized structure
     *
     * @param serializedShaderParameterCollection Serialized collection of
     * shader parameters
     * @return Unique pointer to the created ShaderParameterCollection
     */
    [[nodiscard]] auto
    FromSerialized(const nlohmann::json& serializedShaderParameterCollection)
      const -> std::unique_ptr<IShaderParameterCollection> override;
  };
} // namespace Dwarf