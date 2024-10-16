#pragma once

#include "Core/Rendering/Material.h"
#include "Core/Asset/AssetMetaData.h"

namespace Dwarf
{

  /// @brief Utilities for importing Materials and exporting them to disk.
  class MaterialSerializer
  {
  public:
    /// @brief Deserializes a material file at a given path and creates a
    /// Material instance with meta data.
    /// @param path Path to the material.
    /// @return Imported material instance.
    static std::shared_ptr<Material>
    Deserialize(std::filesystem::path const& path);

    /// @brief Exports a material.
    /// @param material Material instance to export.
    /// @param path Path where the material should be saved to.
    static void
    Serialize(Material const& material, std::filesystem::path const& path);
  };
}