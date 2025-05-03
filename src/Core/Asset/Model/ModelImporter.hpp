#pragma once

#include "Core/Asset/Metadata/IAssetMetadata.hpp"
#include "Core/Asset/Model/IModelImporter.hpp"
#include "Core/Rendering/Mesh/IMeshFactory.hpp"
#include "Logging/IDwarfLogger.hpp"
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

    /**
     * @brief Processes an assimp node
     *
     * @param node Assimp node to process
     * @param scene Assimp scene
     * @param meshes Mesh vector reference to store the found meshes
     * @param parentTransform Transformation matrix of the parent node
     */
    void
    ProcessNode(const aiNode*                        node,
                const aiScene*                       scene,
                std::vector<std::shared_ptr<IMesh>>& meshes,
                glm::mat4                            parentTransform);

    /**
     * @brief Processes an assimp mesh
     *
     * @param mesh Assimp mesh to process
     * @param meshes Mesh vector reference to store the mesh
     * @param transform Transformation matrix for the mesh
     */
    void
    ProcessMesh(const aiMesh*                        mesh,
                std::vector<std::shared_ptr<IMesh>>& meshes,
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
      -> std::vector<std::shared_ptr<IMesh>> override;
  };
}