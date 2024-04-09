#pragma once

#include "Core/Base.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include <filesystem>

namespace Dwarf
{

    class TgaUtils
    {
    public:
        static Ref<TextureData> LoadFromPath(std::filesystem::path const &path);
    };
}