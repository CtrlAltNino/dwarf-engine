#pragma once
#include "Core/Rendering/Mesh/Mesh.h"

namespace Dwarf
{
  class IModelImporter
  {
  public:
    virtual ~IModelImporter() = default;

    // @brief Imports a model.
    /// @param path Path to the model.
    /// @return List of the imported meshes of a model.
    virtual std::vector<std::shared_ptr<Mesh>>
    Import(std::filesystem::path const& path);
  };
}