#pragma once

#include "Core/Rendering/Mesh/IMesh.hpp"

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
      -> std::vector<std::shared_ptr<IMesh>> = 0;
  };
}