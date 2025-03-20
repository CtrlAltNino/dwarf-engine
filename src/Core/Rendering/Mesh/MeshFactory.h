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

    std::unique_ptr<IMesh>
    Create(const std::vector<Vertex>&      vertices,
           const std::vector<unsigned int> indices,
           unsigned int                    materialIndex) override;

    std::unique_ptr<IMesh>
    CreateUnitSphere(int stacks, int slices) override;

    std::unique_ptr<IMesh>
    CreateUnitCube() override;

    std::unique_ptr<IMesh>
    CreateUnitQuad() override;

    std::unique_ptr<IMesh>
    CreateFullscreenQuad() override;

    std::unique_ptr<IMesh>
    MergeMeshes(const std::vector<std::unique_ptr<IMesh>>& meshes) override;

    std::unique_ptr<IMesh>
    MergeMeshes(
      const std::vector<std::reference_wrapper<IMesh>>& meshes) override;
  };
}