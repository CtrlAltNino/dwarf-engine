#pragma once

#include "Core/Rendering/Mesh/IMesh.h"

namespace Dwarf
{
  class IModelImporter
  {
  public:
    virtual ~IModelImporter() = default;

    // @brief Imports a model.
    /// @param path Path to the model.
    /// @return List of the imported meshes of a model.
    virtual auto
    Import(const std::filesystem::path& path)
      -> std::vector<std::unique_ptr<IMesh>> = 0;
  };
}