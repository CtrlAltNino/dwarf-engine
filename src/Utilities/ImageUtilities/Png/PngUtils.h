#pragma once

#include "Core/Base.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include <spng.h>
#include <filesystem>

namespace Dwarf
{

    class PngUtils
    {
    public:
        static Ref<TextureData> LoadFromPath(std::filesystem::path const &path);
    };
}