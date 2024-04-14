#pragma once

#include "Core/Base.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include <filesystem>

namespace Dwarf {

  class HdrUtils
  {
  public:
    static Ref<TextureContainer> LoadFromPath(
      std::filesystem::path const& path);
  };
} // namespace Dwarf