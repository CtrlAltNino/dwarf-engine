#pragma once
#include "../../utilities/dpch.h"
#include "../data structures/Texture.h"

class TextureImporter {
    public:
        static Texture Import(std::string path, nlohmann::json metaData) {
            Texture imported;

            return imported;
        }
};