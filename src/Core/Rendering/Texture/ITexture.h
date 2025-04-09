#pragma once

#include "Utilities/ImageUtilities/TextureCommon.h"

namespace Dwarf
{
  /**
   * @brief Class that abstracts a texture that lives on the GPU
   *
   */
  class ITexture
  {
  public:
    virtual ~ITexture() = default;

    /**
     * @brief Gets the resolution of the texture. A variadic return type
     * depending on the dimensions of the texture.
     *
     * @return Resolution of the texture
     */
    [[nodiscard]] virtual auto
    GetSize() const -> TextureResolution = 0;

    /**
     * @brief Gets the id of the texture
     *
     * @return Id representing the texture on the GPU
     */
    [[nodiscard]] virtual auto
    GetTextureID() const -> uintptr_t = 0;

    auto
    operator==(const ITexture& other) const -> bool
    {
      return GetTextureID() == other.GetTextureID();
    }

    auto
    operator!=(const ITexture& other) const -> bool
    {
      return GetTextureID() != other.GetTextureID();
    }
  };
} // namespace Dwarf