#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.hpp"
#include "IMaterial.hpp"

namespace Dwarf
{
  /**
   * @brief Factory class to create material instances
   *
   */
  class IMaterialFactory
  {
  public:
    virtual ~IMaterialFactory() = default;

    /**
     * @brief Creates a default material
     *
     * @return Unique pointer to the created default material
     */
    [[nodiscard]] virtual auto
    CreateDefaultMaterial() const -> std::unique_ptr<IMaterial> = 0;

    /**
     * @brief Creates a new material which uses the provided shader
     *
     * @param shader Shader to use in the material
     * @return Unique pointer to the created material
     */
    [[nodiscard]] virtual auto
    CreateMaterial(std::unique_ptr<IShaderSourceCollection> shaderSources) const
      -> std::unique_ptr<IMaterial> = 0;

    /**
     * @brief Creates a material instance based on serialized material data
     *
     * @param serializedMaterial Serialized material data
     * @return Unique pointer to the deserialized material
     */
    [[nodiscard]] virtual auto
    FromSerialized(const nlohmann::json& serializedMaterial) const
      -> std::unique_ptr<IMaterial> = 0;
  };
} // namespace Dwarf