#pragma once

#include "Core/Base.h"
#include "Core/Rendering/Mesh/IMeshFactory.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "Logging/IDwarfLogger.h"
namespace Dwarf
{
  class MeshFactory : public IMeshFactory
  {
  private:
    GraphicsApi                   m_GraphicsApi;
    std::shared_ptr<IDwarfLogger> m_Logger;
    std::shared_ptr<IVramTracker> m_VramTracker;

  public:
    MeshFactory(GraphicsApi                   graphicsApi,
                std::shared_ptr<IDwarfLogger> logger,
                std::shared_ptr<IVramTracker> vramTracker);
    ~MeshFactory() override;

    std::unique_ptr<IMesh>
    CreateMesh(const std::vector<Vertex>&       vertices,
               const std::vector<unsigned int>& indices,
               unsigned int                     materialIndex) override;

    std::unique_ptr<IMesh>
    CreateUnitSphere(int stacks, int slices) override;

    std::unique_ptr<IMesh>
    CreateUnitCube() override;

    std::unique_ptr<IMesh>
    CreateUnitQuad() override;
  };
}