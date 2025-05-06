#pragma once

#include "IMeshFactory.hpp"
#include "Logging/IDwarfLogger.hpp"

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
    Create(const std::vector<Vertex>&   vertices,
           const std::vector<uint32_t>& indices,
           uint32_t materialIndex) const -> std::shared_ptr<IMesh> override;

    /**
     * @brief Creates a mesh representing a unit sphere
     *
     * @param stacks Vertical resolution of the unit sphere
     * @param slices Horizontal resolution of the unit sphere
     * @return Unique pointer to the created unit sphere mesh
     */
    [[nodiscard]] auto
    CreateUnitSphere(int stacks, int slices) const
      -> std::shared_ptr<IMesh> override;

    /**
     * @brief Creates a mesh representing a unit cube
     *
     * @return Unique pointer to the created unit cube mesh
     */
    [[nodiscard]] auto
    CreateUnitCube() const -> std::shared_ptr<IMesh> override;

    [[nodiscard]] auto
    CreateSkyboxCube() const -> std::shared_ptr<IMesh> override;

    /**
     * @brief Creates a mesh representing a unit quad
     *
     * @return Unique pointer to the created unit quad mesh
     */
    [[nodiscard]] auto
    CreatePlane() const -> std::shared_ptr<IMesh> override;

    /**
     * @brief Creates a mesh representing a unit quad
     *
     * @return Unique pointer to the created unit quad mesh
     */
    [[nodiscard]] auto
    CreatePreviewQuad() const -> std::shared_ptr<IMesh> override;

    /**
     * @brief Creates a mesh representing a fullscreen quad
     *
     * @return Unique pointer to the created fullscreen quad mesh
     */
    [[nodiscard]] auto
    CreateFullscreenQuad() const -> std::shared_ptr<IMesh> override;

    /**
     * @brief Merges a vector of meshes into one
     *
     * @param meshes Vector of meshes to merge
     * @return Unique pointer to the merged mesh instance
     */
    [[nodiscard]] auto
    MergeMeshes(const std::vector<std::shared_ptr<IMesh>>& meshes) const
      -> std::shared_ptr<IMesh> override;
  };
}