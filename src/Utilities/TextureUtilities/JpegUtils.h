#pragma once

#include "Core/Base.h"
#include "TextureCommon.h"
#include <turbojpeg.h>
#include <filesystem>

namespace Dwarf
{

    class JpegUtils
    {
    public:
        static Ref<TextureData> LoadFromPath(std::filesystem::path const &path);
    };
}