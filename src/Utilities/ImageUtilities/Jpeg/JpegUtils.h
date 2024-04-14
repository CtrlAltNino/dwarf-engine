#pragma once

#include "Core/Base.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include <filesystem>
#include <turbojpeg.h>

namespace Dwarf {

class JpegUtils {
public:
  static Ref<TextureContainer> LoadFromPath(std::filesystem::path const &path);
};
} // namespace Dwarf