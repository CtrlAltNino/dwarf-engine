#pragma once
#include "pch.h"
#include <assimp/scene.h>
#include "Core/Rendering/Mesh.h"

namespace Dwarf
{

  /// @brief Utilities for importing models.
  class ModelImporter
  {
  public:
    // @brief Imports a model.
    /// @param path Path to the model.
    /// @return List of the imported meshes of a model.
    static std::vector<std::shared_ptr<Mesh>>
    Import(std::filesystem::path const& path);

  private:
    static std::vector<std::shared_ptr<Mesh>>
    ProcessNode(const aiNode* node, const aiScene* scene);

    static std::shared_ptr<Mesh>
    ProcessMesh(const aiMesh* mesh, const aiScene* scene);
  };
}