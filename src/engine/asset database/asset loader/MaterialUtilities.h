#pragma once
#include "../../../utilities/dpch.h"
#include "../../rendering/Material.h"
#include "../AssetMetaDataUtilities.h"

/// @brief Utilities for importing Materials and exporting them to disk.
class MaterialUtilities {
    public:
        /// @brief Imports a material.
        /// @param path Path to the material.
        /// @return Imported material instance.
        static Material Import(std::filesystem::path path) {
            Material imported;

            nlohmann::json metaData = AssetMetaDataUtilities::GetMetaData(path);

            return imported;
        }

        /// @brief Exports a material.
        /// @param material Material instance to export.
        /// @param path Path where the material should be saved to.
        static void Export(Material material, std::filesystem::path path) {

        }
};