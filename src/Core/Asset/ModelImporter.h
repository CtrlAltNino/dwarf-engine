#pragma once
#include "Core/Asset/IModelImporter.h"
#include <assimp/scene.h>
#include "Core/Rendering/Mesh/Mesh.h"
#include "Core/Asset/IAssetMetadata.h"
#include "IAssetMetadata.h"

namespace Dwarf
{

  /// @brief Utilities for importing models.
  class ModelImporter : public IModelImporter
  {
  public:
    ModelImporter(std::shared_ptr<IAssetMetadata> assetMetadata);
    // @brief Imports a model.
    /// @param path Path to the model.
    /// @return List of the imported meshes of a model.
    std::vector<std::shared_ptr<Mesh>>
    Import(std::filesystem::path const& path) override;

  private:
    std::shared_ptr<IAssetMetadata> m_AssetMetadata;
    std::vector<std::shared_ptr<Mesh>>
    ProcessNode(const aiNode* node, const aiScene* scene);

    std::shared_ptr<Mesh>
    ProcessMesh(const aiMesh* mesh, const aiScene* scene);
  };
}