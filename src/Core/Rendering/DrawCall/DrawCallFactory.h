#pragma once

#include "Core/Rendering/MeshBuffer/MeshBufferWorker/IMeshBufferWorker.h"
#include "IDrawCall.h"
#include "IDrawCallFactory.h"
#include "Logging/IDwarfLogger.h"
namespace Dwarf
{
  class DrawCallFactory : public IDrawCallFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>      m_Logger;
    std::shared_ptr<IMeshBufferWorker> m_MeshBufferWorker;

  public:
    DrawCallFactory(std::shared_ptr<IDwarfLogger>      logger,
                    std::shared_ptr<IMeshBufferWorker> meshBufferWorker);
    ~DrawCallFactory() override = default;

    std::unique_ptr<IDrawCall>
    Create(std::unique_ptr<IMesh>& mesh,
           IMaterial&              material,
           TransformComponent&     transform) override;
  };
}