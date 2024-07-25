#pragma once
#include "pch.h"
#include "Utilities/ImageUtilities/TextureCommon.h"

namespace Dwarf
{
  class ITexture
  {
  public:
    virtual ~ITexture() = default;

    virtual TextureResolution
    GetSize() const = 0;

    virtual uintptr_t
    GetTextureID() = 0;
  };
} // namespace Dwarf