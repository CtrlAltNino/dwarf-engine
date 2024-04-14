#pragma once

#include "Core/Base.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include <filesystem>
#include <spng.h>

namespace Dwarf {

class PngUtils {
public:
  static Ref<TextureContainer> LoadFromPath(std::filesystem::path const &path);
};
} // namespace Dwarf