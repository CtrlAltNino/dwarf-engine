#pragma once
#include "Core/Rendering/Material.h"

namespace Dwarf
{
  class IMaterialSerializer
  {
  public:
    /// @brief Deserializes a material file at a given path and creates a
    /// Material instance with meta data.
    /// @param path Path to the material.
    /// @return Imported material instance.
    virtual std::shared_ptr<Material>
    Deserialize(std::filesystem::path const& path) = 0;

    /// @brief Exports a material.
    /// @param material Material instance to export.
    /// @param path Path where the material should be saved to.
    virtual void
    Serialize(Material const& material, std::filesystem::path const& path) = 0;
  };
}