#pragma once
#include "../../utilities/dpch.h"
#include "../rendering/Material.h"

class MaterialImporter {
    public:
        static Material Import(std::filesystem::path path, nlohmann::json metaData) {
            Material imported;

            return imported;
        }
};