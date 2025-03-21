#pragma once

#include "IMesh.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class Mesh : public IMesh
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    std::vector<Vertex>           mVertices;
    std::vector<unsigned int>     mIndices;
    unsigned int                  mMaterialIndex = 0;

  public:
    Mesh(const std::vector<Vertex>&       vertices,
         const std::vector<unsigned int>& indices,
         unsigned int                     materialIndex,
         std::shared_ptr<IDwarfLogger>    logger);
    ~Mesh() override;

    /**
     * @brief Retrieves the material index of the mesh
     *
     * @return Material index
     */
    [[nodiscard]] auto
    GetMaterialIndex() const -> unsigned int override;

    /**
     * @brief Returns a reference to the stored vertex list
     *
     * @return Immutable reference to the stored vertex vector
     */
    [[nodiscard]] auto
    GetVertices() const -> const std::vector<Vertex>& override;

    /**
     * @brief Returns a reference to the stored index list
     *
     * @return Immutable reference to the stored index vector
     */
    [[nodiscard]] auto
    GetIndices() const -> const std::vector<unsigned int>& override;

    /**
     * @brief Clones the Mesh instance
     *
     * @return Unique pointer to the cloned Mesh instance
     */
    [[nodiscard]] auto
    Clone() const -> std::unique_ptr<IMesh> override;
  };
}