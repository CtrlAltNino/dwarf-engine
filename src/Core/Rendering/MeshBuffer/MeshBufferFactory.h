#pragma once

#include "Core/Base.h"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class MeshBufferFactory : public IMeshBufferFactory
  {
  private:
    GraphicsApi                   mGraphicsApi;
    std::shared_ptr<IDwarfLogger> mLogger;
    std::shared_ptr<IVramTracker> mVramTracker;

  public:
    MeshBufferFactory(GraphicsApi                   graphicsApi,
                      std::shared_ptr<IDwarfLogger> logger,
                      std::shared_ptr<IVramTracker> vramTracker);
    ~MeshBufferFactory() override;

    /**
     * @brief Creates a mesh buffer on the gpu based on a provided mesh
     *
     * @param mesh Mesh to upload to the gpu
     * @return Unique pointer to the created mesh buffer
     */
    [[nodiscard]] auto
    Create(const std::unique_ptr<IMesh>& mesh) const
      -> std::unique_ptr<IMeshBuffer> override;
  };
}