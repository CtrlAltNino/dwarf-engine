#pragma once

#include "Core/Base.hpp"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.hpp"
#include "Core/Rendering/VramTracker/IVramTracker.hpp"
#include "Logging/IDwarfLogger.hpp"

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
    Create(const std::shared_ptr<IMesh>& mesh) const
      -> std::unique_ptr<IMeshBuffer> override;
  };
}