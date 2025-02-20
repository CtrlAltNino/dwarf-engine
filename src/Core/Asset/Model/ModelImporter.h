#pragma once
#include "Core/Asset/Model/IModelImporter.h"
#include <assimp/scene.h>
#include "Core/Asset/Metadata/IAssetMetadata.h"
#include "Core/Rendering/Mesh/IMeshFactory.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  /// @brief Utilities for importing models.
  class ModelImporter : public IModelImporter
  {
  public:
    ModelImporter(std::shared_ptr<IDwarfLogger>   logger,
                  std::shared_ptr<IAssetMetadata> assetMetadata,
                  std::shared_ptr<IMeshFactory>   meshFactory);
    // @brief Imports a model.
    /// @param path Path to the model.
    /// @return List of the imported meshes of a model.
    std::vector<std::unique_ptr<IMesh>>
    Import(const std::filesystem::path& path) override;

  private:
    std::shared_ptr<IDwarfLogger>   m_Logger;
    std::shared_ptr<IAssetMetadata> m_AssetMetadata;
    std::shared_ptr<IMeshFactory>   m_MeshFactory;

    void
    ProcessNode(const aiNode*                        node,
                const aiScene*                       scene,
                std::vector<std::unique_ptr<IMesh>>& meshes,
                glm::mat4                            parentTransform);

    void
    ProcessMesh(const aiMesh*                        mesh,
                const aiScene*                       scene,
                std::vector<std::unique_ptr<IMesh>>& meshes,
                glm::mat4                            transform);
  };
}