#pragma once
#include "Core/Asset/Metadata/IAssetMetadata.h"
#include "Core/Asset/Model/IModelImporter.h"
#include "Core/Rendering/Mesh/IMeshFactory.h"
#include "Logging/IDwarfLogger.h"
#include <assimp/scene.h>

namespace Dwarf
{
  /// @brief Utilities for importing models.
  class ModelImporter : public IModelImporter
  {
  private:
    std::shared_ptr<IDwarfLogger>   mLogger;
    std::shared_ptr<IAssetMetadata> mAssetMetadata;
    std::shared_ptr<IMeshFactory>   mMeshFactory;

    void
    ProcessNode(const aiNode*                        node,
                const aiScene*                       scene,
                std::vector<std::unique_ptr<IMesh>>& meshes,
                glm::mat4                            parentTransform);

    void
    ProcessMesh(const aiMesh*                        mesh,
                std::vector<std::unique_ptr<IMesh>>& meshes,
                glm::mat4                            transform);

  public:
    ModelImporter(std::shared_ptr<IDwarfLogger>   logger,
                  std::shared_ptr<IAssetMetadata> assetMetadata,
                  std::shared_ptr<IMeshFactory>   meshFactory);
    // @brief Imports a model.
    /// @param path Path to the model.
    /// @return List of the imported meshes of a model.
    auto
    Import(const std::filesystem::path& path)
      -> std::vector<std::unique_ptr<IMesh>> override;
  };
}