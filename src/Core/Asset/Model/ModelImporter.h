#pragma once
#include "Core/Asset/Model/IModelImporter.h"
#include <assimp/scene.h>
#include "Core/Asset/Metadata/IAssetMetadata.h"
#include "Core/Rendering/Mesh/IMeshFactory.h"

namespace Dwarf
{
  /// @brief Utilities for importing models.
  class ModelImporter : public IModelImporter
  {
  public:
    ModelImporter(std::shared_ptr<IAssetMetadata> assetMetadata,
                  std::shared_ptr<IMeshFactory>   meshFactory);
    // @brief Imports a model.
    /// @param path Path to the model.
    /// @return List of the imported meshes of a model.
    std::vector<std::shared_ptr<IMesh>>
    Import(std::filesystem::path const& path) override;

  private:
    std::shared_ptr<IAssetMetadata> m_AssetMetadata;
    std::shared_ptr<IMeshFactory>   m_MeshFactory;
    std::vector<std::shared_ptr<IMesh>>
    ProcessNode(const aiNode* node, const aiScene* scene);

    std::shared_ptr<IMesh>
    ProcessMesh(const aiMesh* mesh, const aiScene* scene);
  };
}