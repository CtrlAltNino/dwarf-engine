#pragma once

#include "Core/Base.h"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "Logging/IDwarfLogger.h"
#include <cstdint>
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

    std::unique_ptr<IMeshBuffer>
    Create(std::unique_ptr<IMesh>& mesh) override;
  };
}