#pragma once

#include "IMaterialSerializer.h"
#include "Core/Rendering/Material/Material.h"

namespace Dwarf
{

  /// @brief Utilities for importing Materials and exporting them to disk.
  class MaterialSerializer : IMaterialSerializer
  {
  public:
    /// @brief Deserializes a material file at a given path and creates a
    /// Material instance with meta data.
    /// @param path Path to the material.
    /// @return Imported material instance.
    std::shared_ptr<Material>
    Deserialize(std::filesystem::path const& path) override;

    /// @brief Exports a material.
    /// @param material Material instance to export.
    /// @param path Path where the material should be saved to.
    void
    Serialize(Material const&              material,
              std::filesystem::path const& path) override;
  };
}