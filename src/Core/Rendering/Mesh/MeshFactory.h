#pragma once

#include "IMeshFactory.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class MeshFactory : public IMeshFactory
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;

  public:
    MeshFactory(std::shared_ptr<IDwarfLogger> logger);
    ~MeshFactory() override = default;

    /**
     * @brief Creates a mesh instance based on the given data
     *
     * @param vertices Vertices of the mesh
     * @param indices Indices of the mesh
     * @param materialIndex Material index of the mesh
     * @return Unique pointer to the created mesh instance
     */
    [[nodiscard]] auto
    Create(const std::vector<Vertex>&       vertices,
           const std::vector<unsigned int>& indices,
           unsigned int materialIndex) const -> std::unique_ptr<IMesh> override;

    /**
     * @brief Creates a mesh representing a unit sphere
     *
     * @param stacks Vertical resolution of the unit sphere
     * @param slices Horizontal resolution of the unit sphere
     * @return Unique pointer to the created unit sphere mesh
     */
    [[nodiscard]] auto
    CreateUnitSphere(int stacks, int slices) const
      -> std::unique_ptr<IMesh> override;

    /**
     * @brief Creates a mesh representing a unit cube
     *
     * @return Unique pointer to the created unit cube mesh
     */
    [[nodiscard]] auto
    CreateUnitCube() const -> std::unique_ptr<IMesh> override;

    /**
     * @brief Creates a mesh representing a unit quad
     *
     * @return Unique pointer to the created unit quad mesh
     */
    [[nodiscard]] auto
    CreatePlane() const -> std::unique_ptr<IMesh> override;

    /**
     * @brief Creates a mesh representing a unit quad
     *
     * @return Unique pointer to the created unit quad mesh
     */
    [[nodiscard]] auto
    CreatePreviewQuad() const -> std::unique_ptr<IMesh> override;

    /**
     * @brief Creates a mesh representing a fullscreen quad
     *
     * @return Unique pointer to the created fullscreen quad mesh
     */
    [[nodiscard]] auto
    CreateFullscreenQuad() const -> std::unique_ptr<IMesh> override;

    /**
     * @brief Merges a vector of meshes into one
     *
     * @param meshes Vector of meshes to merge
     * @return Unique pointer to the merged mesh instance
     */
    [[nodiscard]] auto
    MergeMeshes(const std::vector<std::unique_ptr<IMesh>>& meshes) const
      -> std::unique_ptr<IMesh> override;

    /**
     * @brief Merges a vector of mesh references into one
     *
     * @param meshes Vector of mesh references to merge
     * @return Unique pointer to the merged mesh instance
     */
    [[nodiscard]] auto
    MergeMeshes(const std::vector<std::reference_wrapper<IMesh>>& meshes) const
      -> std::unique_ptr<IMesh> override;
  };
}