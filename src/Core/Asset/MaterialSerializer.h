#pragma once

#include "Core/Rendering/Material.h"
#include "Core/Asset/AssetMetaData.h"

namespace Dwarf
{

    /// @brief Utilities for importing Materials and exporting them to disk.
    class MaterialSerializer
    {
    public:
        /// @brief Deserializes a material file at a given path and creates a Material instance with meta data.
        /// @param path Path to the material.
        /// @return Imported material instance.
        static Ref<Material> Deserialize(std::filesystem::path path);

        /// @brief Exports a material.
        /// @param material Material instance to export.
        /// @param path Path where the material should be saved to.
        static void Serialize(Material material, std::filesystem::path path);
    };
}