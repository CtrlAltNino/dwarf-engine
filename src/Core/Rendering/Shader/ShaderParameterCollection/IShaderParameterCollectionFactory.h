#pragma once
#include "Core/Rendering/Shader/IShader.h"
#include "IShaderParameterCollection.h"

namespace Dwarf
{
  /**
   * @brief Class to construct ShaderParameterCollection instances
   *
   */
  class IShaderParameterCollectionFactory
  {
  public:
    virtual ~IShaderParameterCollectionFactory() = default;

    /**
     * @brief Creates an empty ShaderParameterCollection
     *
     * @return Unique pointer to the created ShaderParameterCollection
     */
    [[nodiscard]] virtual auto
    Create() const -> std::unique_ptr<IShaderParameterCollection> = 0;

    /**
     * @brief Creates a ShaderParameterCollection from a serialized structure
     *
     * @param serializedShaderParameterCollection Serialized collection of
     * shader parameters
     * @return Unique pointer to the created ShaderParameterCollection
     */
    [[nodiscard]] virtual auto
    FromSerialized(const nlohmann::json& serializedShaderParameterCollection)
      const -> std::unique_ptr<IShaderParameterCollection> = 0;
  };
} // namespace Dwarf