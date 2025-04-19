#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.hpp"
#include "Utilities/ISerializable.hpp"

namespace Dwarf
{
  /**
   * @brief Class the contains a set of shader source assets
   *
   */
  class IShaderAssetSourceContainer : public ISerializable
  {
  public:
    virtual ~IShaderAssetSourceContainer() = default;

    /**
     * @brief Gets the stores assets as a ShaderSourceCollection
     *
     * @return Unique pointer to the created ShaderSourceCollection instance
     */
    virtual auto
    GetShaderSources() -> std::unique_ptr<IShaderSourceCollection> = 0;

    auto
    Serialize() -> nlohmann::json override = 0;
  };
}