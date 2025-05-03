#pragma once

#include "Core/Rendering/MeshBuffer/MeshBufferWorker/IMeshBufferWorker.hpp"
#include "IDrawCall.hpp"
#include "IDrawCallFactory.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class DrawCallFactory : public IDrawCallFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>      mLogger;
    std::shared_ptr<IMeshBufferWorker> mMeshBufferWorker;

  public:
    DrawCallFactory(std::shared_ptr<IDwarfLogger>      logger,
                    std::shared_ptr<IMeshBufferWorker> meshBufferWorker);
    ~DrawCallFactory() override;

    /**
     * @brief Creates a draw call from the needed data.
     *
     * @param mesh The mesh data to use for the draw call
     * @param material The material to use for the draw call
     * @param transform The transform to use for the draw call
     * @return Unique pointer to the created draw call
     */
    auto
    Create(std::shared_ptr<IMesh>&  mesh,
           MaterialAsset&           material,
           TransformComponentHandle transform)
      -> std::shared_ptr<IDrawCall> override;
  };
}